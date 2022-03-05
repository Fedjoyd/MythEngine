#pragma once

#include "define.h"

#include <string>
#include <typeinfo>

namespace Ressources
{
	class Ressource
	{
	public:
		Ressource(const std::string& p_name) : m_name(p_name), m_loaded(false) {}
		virtual ~Ressource() {}

		// surcharge OBLIGATOIRE : const std::type_info& GetRessourceType() const override { return typeid(*this); }
		virtual const std::type_info& GetRessourceType() const = 0;

		const std::string& GetName() const { return m_name; }
		bool IsLoaded() const { return m_loaded; }

		virtual void LoadRessource() { m_loaded = true; }
		virtual void UnloadRessource() { m_loaded = false; }
#ifdef MODE_EDITOR
		// don't forget preprocessor : #ifdef MODE_EDITOR and #endif
		virtual void ShowEditorControl() {}
#endif // MODE_EDITOR

	protected:
		bool m_loaded;

	private:
		std::string m_name;
	};
}