#include "Core/System/Time.h"

#include <GLFW/glfw3.h>

Core::System::Time Core::System::Time::singleton = Core::System::Time();

Core::System::Time::Time() :
	m_lastTime(0.f),
	m_timeScale(1.f),
	m_deltaTime(0.f),
	m_targetFrameRate(TARGET_FRAMERATE),
	m_fixedDeltaTime(1.f / TARGET_FRAMERATE),
	m_fixCount(1)
{
}

void Core::System::Time::ResetLastTime()
{
	m_lastTime = (float)glfwGetTime();
}

float curTime = 0.f;
float tempDeltaTime = 0.f;

void Core::System::Time::Update()
{
	curTime = (float)glfwGetTime();

	m_deltaTime = curTime - m_lastTime;
	m_lastTime = curTime;

	m_fixCount = 0;
	tempDeltaTime = m_deltaTime;
	while (tempDeltaTime > m_fixedDeltaTime)
	{
		tempDeltaTime -= m_fixedDeltaTime;
		++m_fixCount;
	}
}
