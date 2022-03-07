#include "Ressources/RessourcesManager.h"

#include "imgui.h"

bool Ressources::RessourcesManager::RegisterAndLoadRessource(std::shared_ptr<Ressource> p_newRessource)
{
	if (m_ressources.find(p_newRessource->GetName()) != m_ressources.end())
		return false;

	p_newRessource->LoadRessource();
	m_ressources.insert({ p_newRessource->GetName(), p_newRessource });

	return true;
}

bool Ressources::RessourcesManager::RegisterAndLoadRessource(Ressource* p_newRessource)
{
	if (m_ressources.find(p_newRessource->GetName()) != m_ressources.end())
		return false;

	p_newRessource->LoadRessource();
	m_ressources.insert({ p_newRessource->GetName(), std::shared_ptr<Ressource>(p_newRessource) });

	return true;
}

void Ressources::RessourcesManager::UnloadUnusedRessource()
{
	for (const std::pair<std::string, std::shared_ptr<Ressource>>& current : m_ressources)
	{
		if (current.second.use_count() == 1)
			current.second->UnloadRessource();
	}
}

std::shared_ptr<Ressources::Scene> Ressources::RessourcesManager::GetScene(const std::string& p_sceneName)
{
	if (m_scenes.find(p_sceneName) == m_scenes.end())
		return nullptr;

	return m_scenes[p_sceneName];
}

bool Ressources::RessourcesManager::AddScene(std::shared_ptr<Scene>& p_newScene)
{
	if (m_scenes.find(p_newScene->GetName()) != m_scenes.end())
		return false;

	p_newScene->SetRessourcesManager(this);
	m_scenes.insert({ p_newScene->GetName(), p_newScene });

	return true;
}

#ifdef MODE_EDITOR
void Ressources::RessourcesManager::ShowManagerWindow(bool* p_opened)
{
	if (p_opened != nullptr)
		if (!(*p_opened))
			return;

	ImGui::Begin("Ressources Manager", p_opened);

	for (const std::pair<std::string, std::shared_ptr<Scene>>& currentScene : m_scenes)
	{
		ImGui::Text((" - " + currentScene.first).c_str());
	}

	ImGui::Separator();

	for (const std::pair<std::string, std::shared_ptr<Ressource>>& currentRessource : m_ressources)
	{
		if (ImGui::Selectable((" - " + currentRessource.first).c_str()))
			m_selected = currentRessource.first;
	}

	ImGui::End();
}

void Ressources::RessourcesManager::ShowEditorWindow(bool* p_opened)
{
	if (p_opened != nullptr)
		if (!(*p_opened))
			return;

	ImGui::Begin("Ressources Editor", p_opened);

	static bool deleteCurrent = false;
	static std::string toDeleteCurrent = "";
	static bool directDelete = false;

	if (!m_selected.empty())
	{
		if (m_ressources.find(m_selected) == m_ressources.end())
			m_selected = "";
		else
		{
			ImGui::Text("Ressource : %s (use count = %ld)", m_ressources[m_selected]->GetName().c_str(), m_ressources[m_selected].use_count());

			if (ImGui::Button("Delete##Ressource"))
			{
				deleteCurrent = true;
				toDeleteCurrent = m_selected;
				if (m_ressources[m_selected].use_count() > 1)
					ImGui::OpenPopup("DeleteRessource?");
				else
					directDelete = true;
			}
			ImGui::SameLine();
			if (m_ressources[m_selected]->IsLoaded())
			{
				if (ImGui::Button("Unload##Ressource"))
					m_ressources[m_selected]->UnloadRessource();
				ImGui::SameLine();
				if (ImGui::Button("Reload##Ressource"))
				{
					m_ressources[m_selected]->UnloadRessource();
					m_ressources[m_selected]->LoadRessource();
				}
			}
			else
				if (ImGui::Button("Load##Ressource"))
					m_ressources[m_selected]->LoadRessource();

			ImGui::Spacing();
			m_ressources[m_selected]->ShowEditorControl();
		}
	}
	else
		ImGui::Text("no Ressource Selected");

	if (deleteCurrent)
	{
		// Always center this window when appearing
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("DeleteRessource?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("%s will be deleted, but he still used by component.\nload and unload while not be process by ressources manager anymore !\n\n", m_ressources[m_selected]->GetName().c_str());
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				directDelete = true;
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				deleteCurrent = false;
			}

			ImGui::EndPopup();
		}

		if (directDelete)
		{
			m_ressources.erase(m_selected);
			deleteCurrent = false;
			directDelete = false;
		}
	}

	ImGui::End();
}
#endif // MODE_EDITOR
