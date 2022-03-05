#include "Core/GameObject.h"

#include "imgui.h"
#include "imgui_stdlib.h"

void Core::GameObject::Start()
{
	for (std::unique_ptr<ECS::Component>& curentComp : m_components)
		curentComp->Start(this);
}

void Core::GameObject::Update()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->Enabled())
			m_components[i]->Update(this);
		
		if (m_components[i]->HasDeleteFlag())
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}

void Core::GameObject::FixedUpdate()
{
	for (std::unique_ptr<ECS::Component>& curentComp : m_components)
		if (curentComp->Enabled())
			curentComp->FixedUpdate(this);
}

#ifdef MODE_EDITOR
void Core::GameObject::EditorUpdate()
{
	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		if (m_components[i]->Enabled())
			m_components[i]->EditorUpdate(this);

		if (m_components[i]->HasDeleteFlag())
		{
			m_components.erase(m_components.begin() + i);
			i--;
		}
	}
}

void Core::GameObject::ShowEditorControl()
{
	ImGui::InputText("##GameObjectName", &m_name);
	ImGui::SameLine();
	if (ImGui::Button("Delete##GameObject"))
		ImGui::OpenPopup("DeleteGameObject?");

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("DeleteGameObject?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("%s will be deleted.\nThis operation cannot be undone !\n\n", m_name.c_str());
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{ 
			ImGui::CloseCurrentPopup();
			SetDeleteFlag(true);
		}
		
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}

	ImGui::Separator();

	for (unsigned int i = 0; i < m_components.size(); i++)
	{
		ImGui::Text("Component : %s", m_components[i]->GetComponentType().name());
		if (ImGui::Button(("Delete##Component_" + std::to_string(i)).c_str()))
			m_components[i]->SetDeleteFlag(true);

		ImGui::Spacing();
		m_components[i]->ShowEditorControl(i);
		ImGui::Separator();
	}
}
#endif

void Core::GameObject::Draw()
{
	for (std::unique_ptr<ECS::Component>& curentComp : m_components)
		if (curentComp->Enabled())
			curentComp->Draw(this);
}
