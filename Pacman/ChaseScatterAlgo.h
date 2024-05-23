#pragma once
#include <vector>
namespace pacman 
{
	class ChaseScatterAlgo final
	{
	public:
		ChaseScatterAlgo() = default;
		~ChaseScatterAlgo() noexcept = default;

		ChaseScatterAlgo(const ChaseScatterAlgo& other) = delete;
		ChaseScatterAlgo(ChaseScatterAlgo&& other) = delete;
		ChaseScatterAlgo& operator=(const ChaseScatterAlgo& other) = delete;
		ChaseScatterAlgo& operator=(ChaseScatterAlgo&& other) = delete;

		void Update();
		bool IsInChaseState() const { return m_IsInChaseState; }

	private:
		bool m_IsInChaseState = false;
		int m_CurrentCycle = 0;
		float m_TotalElapsedTime = 0.0f;
		std::vector<float> m_ChaseScatterDuration = { 7.0f, 20.0f, 7.0f, 20.0f, 5.0f, 20.0f, 5.0f };
	};
}

