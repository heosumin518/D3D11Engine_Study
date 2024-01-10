#include "pch.h"
#include "TimeManager.h"

void Engine::TimeManager::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&m_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&m_prevCount)); // CPU 클럭
}

void Engine::TimeManager::Update()
{
	uint64_t currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	m_deltaTime = (currentCount - m_prevCount) / static_cast<float>(m_frequency);
	m_prevCount = currentCount;

	m_frameCount++;
	m_frameTime += m_deltaTime;

	if (m_frameTime > 1.f)
	{
		m_fps = static_cast<uint32_t>(m_frameCount / m_frameTime);

		m_frameTime = 0.f;
		m_frameCount = 0;
	}

	// 만약에 중단점을 찍고 검사하는 디버깅 상태라면 시간이 흐르는 것을 방지하기 위해 만들어둔 전처리기
#ifdef _DEBUG
	if (m_deltaTime > (1. / 60.))
		m_deltaTime = (1. / 60.);
#endif
}
