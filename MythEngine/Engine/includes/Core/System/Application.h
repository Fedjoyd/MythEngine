#pragma once

#include "define.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui.h"

#include "Core/System/ThreadPool.h"
#include "Ressources/RessourcesManager.h"
#include "Core/GameObjectsManager.h"

#include "Core/Debug/Log.h"
#define MYTH_CURRENT_FILE "Application.cpp"

namespace Core
{
	namespace System
	{
		class Application
		{
		public:
			~Application() {}

			static Application& GetInstance() { return singleton; }

			const std::string& GetName() const { return m_name; }
			int GetWindowWidth() const { return m_width; }
			int GetWindowHeight() const { return m_height; }

			ThreadPool&       GetThreadPool() { return m_threadPool; }
			const ThreadPool& GetThreadPool() const { return m_threadPool; }
			Ressources::RessourcesManager&       GetRessourcesManager() { return m_ressourcesManager; }
			const Ressources::RessourcesManager& GetRessourcesManager() const { return m_ressourcesManager; }
			GameObjectManager&       GetGameObjectManager() { return m_gameObjectManager; }
			const GameObjectManager& GetGameObjectManager() const { return m_gameObjectManager; }

#ifdef MODE_EDITOR
			void ShowSettingsWindow(bool* p_opened);
#endif // MODE_EDITOR

			static void QueryLoadScene(const std::string& p_nextSceneName) { singleton.m_queryLoadScene = true; singleton.m_nextSceneName = p_nextSceneName; }

			static bool Initialize(int p_width, int p_height, const std::string& p_name, bool p_fullscreen = false);
			static bool ShouldExit() { return (singleton.m_glfwWindow == nullptr ? true : glfwWindowShouldClose(singleton.m_glfwWindow) || singleton.m_shouldExit); }
			static void UpdateAndDraw();
			static void Kill();

		private:
			static Application singleton;

			Application() : m_glfwWindow(nullptr), m_shouldExit(false), m_name("none"), m_width(100), m_height(100), m_queryLoadScene(false), m_nextSceneName("") {}

			std::string m_name;
			int         m_width;
			int         m_height;
			GLFWwindow* m_glfwWindow;
			bool        m_shouldExit;

			ThreadPool m_threadPool;
			Ressources::RessourcesManager m_ressourcesManager;
			GameObjectManager m_gameObjectManager;

			// change scene
			bool        m_queryLoadScene;
			std::string m_nextSceneName;

#ifdef MODE_EDITOR
			bool m_editorOpen = true;

			bool m_demoImGuiWindow = false;
			bool m_settingsWindow = true;
			bool m_consoleWindow = true;
			bool m_gameObjectManagerWindow = true;
			bool m_gameObjectEditorrWindow = true;
			bool m_ressourcesManagerWindow = true;
			bool m_ressourceEditorWindow = true;
			bool m_inputEditorWindow = true;

			bool m_run = false;
#endif // MODE_EDITOR
		};
	}
}