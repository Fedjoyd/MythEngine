#pragma once

#include "define.h"

#include <string>
#include <memory>
#include <unordered_map>

#include "Ressource.h"
#include "Scene.h"

namespace Ressources
{
	class RessourcesManager
	{
	public:
		RessourcesManager() : m_ressources(), m_scenes() {}
		~RessourcesManager() {}

		template <class T>
		std::shared_ptr<T> GetRessource(const std::string& p_ressourceName);
		template <class T>
		bool               TryGetRessource(const std::string& p_ressourceName, std::shared_ptr<T>* outRessource);
		bool               RegisterAndLoadRessource(std::shared_ptr<Ressource> p_newRessource);
		bool               RegisterAndLoadRessource(Ressource* p_newRessource);
		void               UnloadUnusedRessource();

		std::shared_ptr<Scene> GetScene(const std::string& p_sceneName);
		bool                   AddScene(std::shared_ptr<Scene>& p_newScene);

#ifdef MODE_EDITOR
		void ShowManagerWindow(bool* p_opened = nullptr);
		void ShowEditorWindow(bool* p_opened = nullptr);
#endif // MODE_EDITOR

 	private:
		std::unordered_map<std::string, std::shared_ptr<Ressource>> m_ressources;
		std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;

#ifdef MODE_EDITOR
		std::string m_selected = "";
#endif // MODE_EDITOR
	};

	template<class T>
	inline std::shared_ptr<T> RessourcesManager::GetRessource(const std::string& p_ressourceName)
	{
		if (m_ressources.find(p_ressourceName) == m_ressources.end())
			return nullptr;
		if (m_ressources[p_ressourceName]->GetRessourceType() != typeid(T))
			return nullptr;

		return std::static_pointer_cast<T, Ressource>(m_ressources[p_ressourceName]);
	}
	template<class T>
	inline bool RessourcesManager::TryGetRessource(const std::string& p_ressourceName, std::shared_ptr<T>* outRessource)
	{
		std::shared_ptr<T> toTestReturn = GetRessource<T>(p_ressourceName);

		if (toTestReturn == nullptr)
			return false;

		if (outRessource != nullptr)
			*outRessource = toTestReturn;

		return true;
	}
}
