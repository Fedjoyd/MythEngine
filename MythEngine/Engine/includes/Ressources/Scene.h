#pragma once

#include "define.h"

#include "Core/GameObject.h"

#include <string>
#include <vector>

namespace Ressources
{
	class RessourcesManager;
}

namespace Ressources
{
	class Scene
	{
	public:
		Scene(const std::string& p_name) : m_ressourceManager(nullptr), m_name(p_name), m_deleteFlag(false) {}
		virtual ~Scene() {}

		void SetRessourcesManager(Ressources::RessourcesManager* p_newRM = nullptr) { m_ressourceManager = p_newRM; }

		const std::string& GetName() const { return m_name; }
		bool HasDeleteFlag() const { return m_deleteFlag; }
		void SetDeleteFlag(bool p_value = true) { m_deleteFlag = p_value; }

		virtual void RegisterGameObjects(std::vector<std::unique_ptr<Core::GameObject>>& p_GameObjectList) = 0;

#ifdef MODE_EDITOR
		// only in editor mode (only when MODE_EDITOR is define)
		virtual bool HasSaveSupport() const { return false; }
		// only in editor mode (only when MODE_EDITOR is define)
		virtual void SaveGameObjects(const std::vector<Core::GameObject>& p_GameObjectList) {}
#endif // MODE_EDITOR

	protected:
		Ressources::RessourcesManager* m_ressourceManager;

	private:
		std::string m_name;
		bool        m_deleteFlag;
	};
}