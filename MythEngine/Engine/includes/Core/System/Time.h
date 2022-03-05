#pragma once

#include "define.h"

namespace Core
{
	namespace System
	{
		class Time
		{
		public:
			~Time() {}

			static inline Time& GetInstance() { return singleton; }

			static inline float DeltaTime() { return singleton.m_deltaTime * singleton.m_timeScale; }
			static inline float FixedDeltaTime() { return singleton.m_fixedDeltaTime * singleton.m_timeScale; }
		//	static inline int   Fixing() { return (singleton.m_fixCount == 0 ? 1 : singleton.m_fixCount); }
			static inline int   Fixing() { return singleton.m_fixCount; }

			inline float& TimeScale() { return m_timeScale; }
			inline float& TargetFrameRate() { return m_targetFrameRate; }

			void ResetLastTime();
			void Update();

		private:
			static Time singleton;

			Time();

			float m_lastTime;
			float m_timeScale;
			float m_deltaTime;
			float m_targetFrameRate;
			float m_fixedDeltaTime;
			int   m_fixCount;
		};
	}
}