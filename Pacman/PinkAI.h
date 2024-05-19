﻿#pragma once
#include "GhostAI.h"
namespace diji
{
	//                                                                        If Pac has               
	// ┌─────────┐Complex Algortihm┌────────────┐ Is inside Maze ┌───────────┐ power up┌──────────┐    
	// │ Waiting ├─────────────────► Enter Maze ├────────────────► Chase Pac ◄─────────► Flee Pac │    
	// └─────────┘                 └────▲───────┘                └───────────┘         └────┬─────┘    
	//                                  │                                                   │          
	//                                  │                                                   │          
	//                                  │                                                   │ If Killed
	//                                  │                        ┌───────────┐              │          
	//                                  │                        │ Return To │              │          
	//                                  └────────────────────────┤   Spawn   ◄──────────────┘          
	//                                                           └───────────┘                         
	class Collider;
	class Transform;

	class PinkAI final : public GhostAI
	{
	public:
		PinkAI(GameObject* ownerPtr, GameObject* player);
		~PinkAI() override = default;

		void Update() override {};
		void FixedUpdate() override;
	};

	class Waiting final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Waiting() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;

	private:
		float tempTimer = 0.0f;
	};

	class ChasePac final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ChasePac() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	};

	class ReturnToSpawn final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ReturnToSpawn() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	};

	class EnterMaze final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~EnterMaze() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(Transform* transform, Collider* collider, Collider* player) override;
	private:
		bool tempLock = false;
	};
}
