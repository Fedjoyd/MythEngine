#include "Core/GameObjectsManager.h"

#include "imgui.h"

void Core::GameObjectManager::LoadScene(std::shared_ptr<Ressources::Scene> p_newScene)
{
	m_currentScene = p_newScene;

	m_gameObjects.clear();

	if (m_currentScene != nullptr)
		m_currentScene->RegisterGameObjects(m_gameObjects);
}

void Core::GameObjectManager::Start()
{
	for (std::unique_ptr<GameObject>& currentGO : m_gameObjects)
		currentGO->Start();
}

void Core::GameObjectManager::Update()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update();

		if (m_gameObjects[i]->HasDeleteFlag())
		{
			m_gameObjects.erase(m_gameObjects.begin() + i);
			i--;
		}
	}
}

void Core::GameObjectManager::FixedUpdate()
{
	for (std::unique_ptr<GameObject>& currentGO : m_gameObjects)
		currentGO->FixedUpdate();
}

#ifdef MODE_EDITOR
void Core::GameObjectManager::EditorUpdate()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->EditorUpdate();

		if (m_gameObjects[i]->HasDeleteFlag())
		{
			m_gameObjects.erase(m_gameObjects.begin() + i);
			i--;
		}
	}
}

void Core::GameObjectManager::ShowManagerWindow(bool* p_opened)
{
	ImGui::Begin("GameObject Manager", p_opened);

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		if (ImGui::Selectable((m_gameObjects[i]->GetName() + "##" + std::to_string(i)).c_str()))
			m_selectedGameObject = i;
	}

	ImGui::End();
}

void Core::GameObjectManager::ShowEditorWindow(bool* p_opened)
{
	ImGui::Begin("GameObject Editor", p_opened);

	if (m_selectedGameObject < m_gameObjects.size())
		m_gameObjects[m_selectedGameObject]->ShowEditorControl();
	else
		ImGui::Text("Aucun GameObject selectionne");

	ImGui::End();
}
#endif // MODE_EDITOR

void Core::GameObjectManager::Draw()
{
	for (std::unique_ptr<GameObject>& currentGO : m_gameObjects)
		currentGO->Draw();
}
