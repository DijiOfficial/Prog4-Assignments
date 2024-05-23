#pragma once
#include "GameActorCommands.h"
#include <glm/glm.hpp>
namespace diji
{
	class Transform;
	class GameObject;
}

namespace pacman
{
	class HealthCounter;
	class ScoreCounter;
	enum class PointType;

	class Move final : public diji::GameActorCommands
	{
	public:
		Move(diji::GameObject* actor, diji::Movement movement);
		~Move() noexcept override = default;

		void Execute() override;

	private:
		const glm::vec2 m_Speed = { 160.f, 160.f }; //sets speed as universal, may change it to get it from the actor
		diji::Movement m_Movement;
		diji::Transform* m_TransformComponentPtr;
	};

	class HitCommand final : public diji::GameActorCommands
	{

	public:
		HitCommand(diji::GameObject* actor);
		~HitCommand() noexcept override = default;

		void Execute() override;

	private:
		HealthCounter* m_HealthComponentPtr;
	};

	class ScoreCommand final : public diji::GameActorCommands
	{

	public:
		ScoreCommand(diji::GameObject* actorPtr, PointType point);
		~ScoreCommand() noexcept override = default;

		void Execute() override;

	private:
		ScoreCounter* m_ScoreComponentPtr;
		PointType m_PointType;
	};
}

