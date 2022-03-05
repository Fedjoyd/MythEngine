#pragma once

#include "define.h"

#include <typeinfo>

namespace Core
{
	class GameObject;
}

namespace ECS
{
	class Component
	{
	public:
		Component() : m_enabled(true), m_deleteFlag(false) {}
		virtual ~Component() {}

		// surcharge OBLIGATOIRE : const std::type_info& GetComponentType() const override { return typeid(*this); }
		virtual const std::type_info& GetComponentType() const = 0;

		bool Enabled() const { return m_enabled; }
		void SetEnabled(bool p_value = true) { m_enabled = p_value; }
		bool HasDeleteFlag() const { return m_deleteFlag; }
		void SetDeleteFlag(bool p_value = true) { m_deleteFlag = p_value; }

		virtual void Start(Core::GameObject* p_gameObject) {}
		virtual void Update(Core::GameObject* p_gameObject) {}
		virtual void FixedUpdate(Core::GameObject* p_gameObject) {}
#ifdef MODE_EDITOR
		// don't forget preprocessor : #ifdef MODE_EDITOR and #endif
		virtual void EditorUpdate(Core::GameObject* p_gameObject) {}
		// don't forget preprocessor : #ifdef MODE_EDITOR and #endif
		virtual void ShowEditorControl(const unsigned int p_indexComponent) {}
#endif // MODE_EDITOR
		virtual void Draw(Core::GameObject* p_gameObject) {}

	private:
		bool m_enabled;
		bool m_deleteFlag;
	};
}