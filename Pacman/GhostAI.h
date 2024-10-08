﻿#pragma once
#include "Component.h"
#include "IObserver.h"
#include "GhostStates.h"
#include "Subject.h"

#include <string>
#include <vector>

namespace diji
{
	class Transform;
	class Collider;
	enum class Movement;
	class Texture;
}

namespace pacman
{ 
	//                                                                      If Power                         
	//                                    If In Chase Mode    ┌───────┐   Pellet Eaten                       
	//                               ┌────────────────────────► Chase ◄──────────────────┐                   
	//                               │                        └───▲───┘   Power Ran Out  │                   
	//                               │                            │                      │                   
	//                               │                            │                      │                   
	//                               │                          If│Time            ┌─────▼──────┐            
	//                               │                          To│Switch          │ Frightened ├──┐         
	//                               │                            │                └─────▲──────┘  │         
	//                               │                            │                      │         │         
	//               Complex         │            If In           │         If Power     │         │         
	// ┌─────────┐  Algorithm  ┌─────┴──────┐  Scatter Mode  ┌────▼────┐  Pellet Eaten   │         │         
	// │ Waiting ├─────────────► Enter Maze ├────────────────► Scatter ◄─────────────────┘         │If       
	// └─────────┘             └─────▲──────┘                └─────────┘  Power Ran Out            │Colliding
	//                               │                                                             │With     
	//                      Personnal│Spawn                                                        │Player   
	//                         Is Reached                                                          │         
	//                               │                                  Update Paused For          │         
	//                          ┌────┴────┐  If Spawn Reached  ┌───────┐  For 2 Seconds ┌───────┐  │         
	//                          │ Respawn ◄────────────────────┤ Eaten ◄────────────────┤ Dying ◄──┘         
	//                          └─────────┘                    └───────┘                └───────┘            
	class PelletObserver;
	class GhostsTimers;
	class GhostAI : public diji::Component, public diji::IObserver, public diji::Subject
	{
	public:
		~GhostAI() noexcept = default;

		GhostAI(const GhostAI& other) = delete;
		GhostAI(GhostAI&& other) = delete;
		GhostAI& operator=(const GhostAI& other) = delete;
		GhostAI& operator=(GhostAI&& other) = delete;

		void Init() override;
		void Update() override;
		void FixedUpdate() override;
		void LateUpdate() override;
		void OnNotify(diji::MessageTypes message, diji::Subject* subject) override;

		void SetInMenu();
		virtual std::unique_ptr<GhostState> GetChaseState() const = 0;
		virtual diji::Transform* GetSecondGhostTransform() const { return nullptr; };

		diji::Transform* GetTransform() const { return m_TransformCompPtr; };
		diji::Transform* GetPlayerTransform() const { return m_PlayerTransformPtr; };
		diji::Collider* GetCollider() const { return m_ColliderCompPtr; };
		diji::Collider* GetPlayerCollider() const { return m_PlayerColliderPtr; };
		diji::Texture* GetTexture() const { return m_TextureCompPtr; };
		bool GetIsInChaseState() const;
		glm::vec2 GetSpawnPoint() const { return m_PersonnalSpawn; };
		glm::vec2 GetScatterTarget() const { return m_ScatterTarget; };
		GhostState* GetCurrentState() const { return m_CurrentStateUPtr.get(); };
		int GetPelletCount() const;
		const diji::GameObject* GetParent() const { return GetOwner(); };

		bool IsFrightened() const { return m_IsFrightened; };
		bool IsPowerAlmostOver() const { return m_PowerUpTimer >= 7.f; };
		bool IsUpdatePaused() const;
		bool IsLastGhostEaten() const { return m_IsLastGhostEaten; };
		bool GetIsInMenu() const { return m_IsInMenu; };
		float GetPowerUpTimer() const { return m_PowerUpTimer; };

		void ClearFrightened() const;
		void SetGhostTexture() const;
		void TurnAround() const;
		void SetIsLastGhostEaten(bool isEaten) const { m_IsLastGhostEaten = isEaten; };
		void SetGhostsVector(const std::vector<GhostAI*>& ghosts) { m_GhostsPtrs = ghosts; };
		void SetGhostKilledPlayer() { m_IsPlayerKilled = true; };
		std::vector<GhostAI*> GetGhostsAI() const { return m_GhostsPtrs; };

		//player controls
		void SetAsPlayer() { m_IsPlayerControlled = true; };
		void SetNextMovement(const diji::Movement movement) { m_NextDirection = movement; };
		bool IsPLayerControlled() const { return m_IsPlayerControlled; };
		diji::Movement GetNextMovement() const { return m_NextDirection; };

	protected:
		GhostAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers);

		std::unique_ptr<GhostState> m_CurrentStateUPtr = nullptr;
		glm::vec2 m_PersonnalSpawn = { 0, 0 };
		glm::vec2 m_ScatterTarget = { 0, 0 };
		std::string m_TexturePath = "";
		int m_PelletsNeededForRespawn = 0;
		bool m_IsInMenu = false;

	private:
		std::vector<GhostAI*> m_GhostsPtrs;
		diji::Collider* m_PlayerColliderPtr;
		diji::Transform* m_PlayerTransformPtr;
		PelletObserver* m_PelletCounterPtr;
		GhostsTimers* m_GhostsTimerPtr;
		diji::Collider* m_ColliderCompPtr;
		diji::Transform* m_TransformCompPtr;
		diji::Texture* m_TextureCompPtr;
		mutable float m_PowerUpTimer = 0.f;
		mutable bool m_IsFrightened = false;
		mutable bool m_IsLastGhostEaten = false;
		bool m_IsPlayerKilled = false;
		float m_PlayerDeathTimer = 0.f;
		const float m_TunnelSpeed = 0.9375f;

		void Reset();

		//player controls
		bool m_IsPlayerControlled = false;
		diji::Movement m_NextDirection;
	};

	class RedAI final : public GhostAI
	{
	public:
		RedAI(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers);
		~RedAI() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

	class Pinky final : public GhostAI
	{
	public:
		Pinky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers);
		~Pinky() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

	class Inky final : public GhostAI
	{
	public:
		Inky(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers, const diji::GameObject* blinky);
		~Inky() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
		diji::Transform* GetSecondGhostTransform() const override { return m_BlinkyTransformPtr; };

	private:
		diji::Transform* m_BlinkyTransformPtr;
	};

	class Clyde final : public GhostAI
	{
	public:
		Clyde(diji::GameObject* ownerPtr, diji::GameObject* player, const diji::GameObject* pelletCounter, const diji::GameObject* timers);
		~Clyde() noexcept = default;

		std::unique_ptr<GhostState> GetChaseState() const override;

		void Init() override;
	};

}

