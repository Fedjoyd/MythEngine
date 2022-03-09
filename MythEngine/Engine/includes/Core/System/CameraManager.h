#pragma once

#include "define.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ECS/Camera.h"

namespace Core
{
	namespace System
	{
		class CameraManager
		{
		public:
			~CameraManager() {}

			static CameraManager& GetInstance() { return singleton; }

			static void ResizeCallback(GLFWwindow* window, int width, int height);
			void Setup(GLFWwindow* window);

			ECS::Camera* GetCamera() { return m_currentCamera; }
			bool HasCamera() { return (m_currentCamera != nullptr); }
			void SetCamera(ECS::Camera* p_newCamera, const bool p_setSelected = true);
			
			float Aspect;
			float FovY;
			float Near;
			float Far;

			static Mat4x4 GetProjectionMatrix();

		private:
			CameraManager();

			static CameraManager singleton;

			ECS::Camera* m_currentCamera;
		};
	}
}