#pragma once

#include "define.h"

#include <string>
#include <vector>
#include <memory>

#include "ECS/Component.h"

// m_components.emplace_back<std::unique_ptr<ECS::Component>>(p_component)

namespace Core
{
	class GameObject
	{
	public:
		GameObject(const std::string& p_name) : m_name(p_name), m_deleteFlag(false), m_components() {}
		~GameObject() {}

		std::string&       GetName() { return m_name; }
		const std::string& GetName() const { return m_name; }
		bool HasDeleteFlag() const { return m_deleteFlag; }
		void SetDeleteFlag(bool p_value = true) { m_deleteFlag = p_value; }

		std::vector<std::unique_ptr<ECS::Component>>&       GetComponents() { return m_components; }
		const std::vector<std::unique_ptr<ECS::Component>>& GetComponents() const { return m_components; }

		template <class T>
		T* GetComponent(const unsigned int indexComponent = 0u);
		template <class T>
		bool TryGetComponent(T** out_component, const unsigned int indexComponent = 0u);
		template <class T>
		std::vector<T*> GetAllComponents();

		void Start();
		void Update();
		void FixedUpdate();
#ifdef MODE_EDITOR
		void EditorUpdate();
		void ShowEditorControl();
#endif // MODE_EDITOR
		void Draw();

	private:
		std::string m_name;
		bool        m_deleteFlag;

		std::vector<std::unique_ptr<ECS::Component>> m_components;
	};

	template<class T>
	inline T* GameObject::GetComponent(const unsigned int indexComponent)
	{
		T* toReturnPtr = nullptr;
		unsigned int currentComponent = 0;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->GetComponentType() == typeid(T))
			{
				toReturnPtr = m_components[i].get();

				if (currentComponent < indexComponent)
					currentComponent++;
				else
					return toReturnPtr;
			}
		}

		return toReturnPtr;
	}

	template<class T>
	inline bool GameObject::TryGetComponent(T** out_component, const unsigned int indexComponent)
	{
		T* toTestReturn = GetComponent<T>(indexComponent);

		if (toTestReturn == nullptr)
			return false;

		if (out_component != nullptr)
			*out_component = toTestReturn;

		return true;
	}

	template<class T>
	inline std::vector<T*> GameObject::GetAllComponents()
	{
		std::vector<T*> toReturnlist;
		unsigned int currentComponent = 0;

		for (unsigned int i = 0; i < m_components.size(); i++)
		{
			if (m_components[i]->GetComponentType() == typeid(T))
				toReturnlist.push_back(m_components[i].get());
		}

		return toReturnlist;
	}
}