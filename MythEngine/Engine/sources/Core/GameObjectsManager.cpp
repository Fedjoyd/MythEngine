#include "Core/GameObjectsManager.h"
#include "ECS/Model.h"
#include "maths/Mat4x4.hpp"
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
	std::vector<Vertex> allVertices;
	for (std::unique_ptr<GameObject>& currentGO : m_gameObjects)
	{
		currentGO->Start();
		ECS::Model* model = currentGO->GetComponent<ECS::Model>();
		if (model != nullptr)
			allVertices.insert(allVertices.end(), model->m_allVertices.begin(), model->m_allVertices.end());
	}

	glGenBuffers(1, &gpu->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, gpu->VBO);
	glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vertex), &allVertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &gpu->VAO);
	glBindVertexArray(gpu->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, gpu->VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
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
	std::vector<Vertex> allVertices;
	for (std::unique_ptr<GameObject>& currentGO : m_gameObjects)
	{
		currentGO->FixedUpdate();
	}
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
	if (p_opened != nullptr)
		if (!(*p_opened))
			return;

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
	if (p_opened != nullptr)
		if (!(*p_opened))
			return;

	ImGui::Begin("GameObject Editor", p_opened);

	if (m_selectedGameObject < m_gameObjects.size())
		m_gameObjects[m_selectedGameObject]->ShowEditorControl();
	else
		ImGui::Text("Aucun GameObject selectionne");

	ImGui::End();
}
#endif // MODE_EDITOR

#include "Core/System/CameraManager.h"

void Core::GameObjectManager::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(gpu->VAO);
	Mat4x4 proj = System::CameraManager::GetInstance().GetProjectionMatrix();

	Mat4x4 view = Mat4x4::ViewRad(0.f, 0.f, { 0.f, 0.f, 5.f });
	if (System::CameraManager::GetInstance().HasCamera())
		view = System::CameraManager::GetInstance().GetCamera()->GetViewMatrix();

	Mat4x4 viewProj = proj * view;

	for (std::unique_ptr<GameObject>& currentGO : m_gameObjects)
	{
		ECS::Model* model = currentGO->GetComponent<ECS::Model>();

		if (model == nullptr)
			continue;

		model->m_shader->use();
		glUniformMatrix4fv(glGetUniformLocation(model->m_shader->Program, "uVP"), 1, GL_FALSE, viewProj.m_mat.data());
		currentGO->Draw();
	}
}
