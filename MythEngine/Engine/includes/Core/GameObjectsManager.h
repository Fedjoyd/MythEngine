#pragma once

#include "define.h"

#include <string>
#include <vector>
#include <memory>

#include "GameObject.h"
#include "Ressources/Scene.h"
#include "glad/glad.h"

// m_currentScene.emplace_back<std::unique_ptr<Core::GameObject>>(p_gameObject)

struct BigVAO
{
	GLuint VAO;
	GLuint VBO;
};

namespace Core
{
	class GameObjectManager
	{
	public:
		GameObjectManager() : m_gameObjects(), m_currentScene(nullptr) { gpu = new BigVAO(); }
		~GameObjectManager() { delete gpu; }

		void LoadScene(std::shared_ptr<Ressources::Scene> p_newScene);
		std::shared_ptr<Ressources::Scene> GetCurrentScene() const { return m_currentScene; }

		std::vector<std::unique_ptr<GameObject>>& GetGameObjects() { return m_gameObjects; }
		const std::vector<std::unique_ptr<GameObject>>& GetGameObjects() const { return m_gameObjects; }

		void Start();
		void Update();
		void FixedUpdate();
#ifdef MODE_EDITOR
		void EditorUpdate();
		void ShowManagerWindow(bool* p_opened = nullptr);
		void ShowEditorWindow(bool* p_opened = nullptr);
#endif // MODE_EDITOR
		void Draw();

	private:
		std::vector<std::unique_ptr<GameObject>> m_gameObjects;
		std::shared_ptr<Ressources::Scene> m_currentScene;
		BigVAO* gpu;

#ifdef MODE_EDITOR
		unsigned int m_selectedGameObject = 0u;
#endif // MODE_EDITOR
	};
}