#include "Core/System/CameraManager.h"

#include "Core/Debug/Log.h"
#define MYTH_CURRENT_FILE "CameraManager.cpp"

Core::System::CameraManager Core::System::CameraManager::singleton;

void Core::System::CameraManager::ResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	if (height == 0)
		return;

	singleton.Aspect = ((float)width / (float)height);

	MYTH_DEBUG("window resize width : %d, height : %d, aspect : %.3f", width, height, singleton.Aspect);
}

void Core::System::CameraManager::Setup(GLFWwindow* window)
{
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	ResizeCallback(window, width, height);

	glfwSetWindowSizeCallback(window, ResizeCallback);
}

void Core::System::CameraManager::SetCamera(ECS::Camera* p_newCamera, const bool p_setSelected)
{
	if (m_currentCamera != nullptr && p_setSelected)
		m_currentCamera->SetSelected(false);

	m_currentCamera = p_newCamera;

	if (m_currentCamera != nullptr && p_setSelected)
		m_currentCamera->SetSelected(true);
}

Mat4x4 Core::System::CameraManager::GetProjectionMatrix()
{
	if (singleton.HasCamera())
		if (singleton.GetCamera()->IsOrthographique())
			return Mat4x4::Orthographique(Maths::DegToRad(singleton.FovY), singleton.Aspect, singleton.Near, singleton.Far);

	return Mat4x4::Perspective(Maths::DegToRad(singleton.FovY), singleton.Aspect, singleton.Near, singleton.Far);
}

Core::System::CameraManager::CameraManager() :
	Aspect(16.f / 9.f),
	FovY(80.f),
	Near(0.001f),
	Far(1000.f),
	m_currentCamera(nullptr)
{
}
