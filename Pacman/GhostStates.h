#pragma once
#include <memory>
#include <map>
#include <glm/vec2.hpp>

namespace diji
{
	enum class Movement;
}

namespace pacman 
{
	class GhostAI;

	class GhostState
	{
	public:
		//todo: set destructors to noexcept
		GhostState() = default;
		virtual ~GhostState() = default;

		GhostState(const GhostState& other) = delete;
		GhostState(GhostState&& other) = delete;
		GhostState& operator=(const GhostState& other) = delete;
		GhostState& operator=(GhostState&& other) = delete;

		virtual void OnEnter([[maybe_unused]] const GhostAI* ghost) = 0;
		virtual void OnExit([[maybe_unused]] const GhostAI* ghost) = 0;
		virtual std::unique_ptr<GhostState> Execute([[maybe_unused]] const GhostAI* ghost) = 0;

	protected:
		void SeekTarget(const GhostAI* ghost, const glm::vec2& target);
		void GoToTarget(const GhostAI* ghost, const glm::vec2& target);
		const glm::vec2 m_SpawnPoint{ 227, 262 };
		const int m_Step = 1;
		bool m_DisplayDirection = true;
	private:
		bool m_TempLock = false;
		int m_LockedFrames = 0;

		void CalculateDirection(const GhostAI* ghost, const glm::vec2& target);
		glm::vec2 GetTargetTranslation(diji::Movement movement) const;

		diji::Movement ChooseRandomDirection(const std::map<diji::Movement, bool>& possibleDirections) const;
	};

	class Eaten final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Eaten() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		std::unique_ptr<GhostState> m_NextStateUPtr;
	};

	class Respawn final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Respawn() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		glm::vec2 m_PersonnalSpawn{ 0, 0 };
	};

	class ExitMaze final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~ExitMaze() override = default;

		void OnEnter(const GhostAI*) override {};
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		const glm::vec2 m_OutsidePosition{ 212, 247 };
		std::unique_ptr<GhostState> m_NextStateUPtr;
	};

	class Scatter final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Scatter() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	private:
		glm::vec2 m_Target{ 0, 0 };
	};

	class Frightened final : public GhostState
	{
	public:
		using GhostState::GhostState;
		~Frightened() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI* ghost) override;
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};

	class Chase : public GhostState
	{
	public:
		using GhostState::GhostState;
		virtual ~Chase() override = default;

		virtual void OnEnter(const GhostAI*) override = 0;
		virtual void OnExit(const GhostAI*) override = 0;
		virtual std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override = 0;
	};

	class RedChase final : public Chase
	{
	public:
		using Chase::Chase;
		~RedChase() override = default;

		void OnEnter(const GhostAI* ghost) override;
		void OnExit(const GhostAI*) override {};
		std::unique_ptr<GhostState> Execute(const GhostAI* ghost) override;
	};
}
