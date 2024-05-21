#pragma once
#include "Subject.h"
#include "Component.h"

namespace diji 
{
	enum class SoundId;
	class GameObject;
	class Collider;
	class Render;
}

namespace pacman
{
	class PelletObserver;
	class PickUp final : public diji::Component, public diji::Subject
	{
	public:
		PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const diji::GameObject* pelletCounter, const int value);
		PickUp(diji::GameObject* ownerPtr, const diji::GameObject* player, const int value);
		~PickUp() override = default;

		PickUp(const PickUp& other) = delete;
		PickUp(PickUp&& other) = delete;
		PickUp& operator=(const PickUp& other) = delete;
		PickUp& operator=(PickUp&& other) = delete;

		void Update() override;
		void FixedUpdate() override {};
		int GetValue() const { return m_Value; };
		void HandleCollision();

	private:
		const int m_Value;
		diji::SoundId m_SoundId;
		diji::Collider* m_PlayerColliderPtr;
		diji::Collider* m_OwnerColliderPtr;
		PelletObserver* m_PelletCounter;
		diji::Render* m_RenderCompPtr;
		bool m_IsPowerUp = false;
		bool m_IsDisabled = false;
		int m_PowerUpInvisibleFrames = 0;
	};
}

