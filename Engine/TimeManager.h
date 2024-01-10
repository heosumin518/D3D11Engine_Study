#pragma once

namespace Engine
{
	class TimeManager
	{
		DECLARE_SINGLE(TimeManager);

	public:
		void Init();
		void Update();

		uint32_t GetFPS() { return m_fps; }
		float GetDeltaTime() { return m_deltaTime; }

	private:
		uint64_t m_frequency = 0;
		uint64_t m_prevCount = 0;
		float m_deltaTime = 0.f;

	private:
		uint32_t m_frameCount = 0;
		float m_frameTime = 0.f;
		uint32_t m_fps = 0;
	};
}


