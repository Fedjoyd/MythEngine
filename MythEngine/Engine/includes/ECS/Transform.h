#pragma once

#include "define.h"

#include "Component.h"

#include "maths/Maths.hpp"
#include "maths/Vec3.hpp"
#include "maths/Quaternion.hpp"
#include "maths/Mat4x4.hpp"

#include <vector>

namespace ECS
{
	class Transform : public Component
	{
	public:
		Transform(Transform* p_parent = nullptr);
		~Transform();

		const std::type_info& GetComponentType() const override { return typeid(*this); }

		Transform* GetParent() const { return m_parent; }
		bool HasParent() const { return (m_parent != nullptr); }
		void SetParent(Transform* p_parent, bool unsetParent = true);
		void AddChild(Transform* p_child);
		void RemChild(Transform* p_child);

		Vec3       Position;
		Quaternion Rotation;
		Vec3       Scale;

		Vec3       GetGlobalPosition() const;
		Quaternion GetGlobalRotation() const;
		Vec3       GetGlobalScale() const;
		Mat4x4     GetGlobalMatrix() const;

		void Update(Core::GameObject* p_gameObject) override;
#ifdef MODE_EDITOR
		void EditorUpdate(Core::GameObject* p_gameObject) override;
		void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // MODE_EDITOR

	private:
		Transform* m_parent;
		std::vector<Transform*> m_childs;

		Vec3       m_oldPosition;
		Quaternion m_oldRotation;
		Vec3       m_oldScale;
		Mat4x4     m_localMatrix;

#ifdef MODE_EDITOR
		static std::vector<std::pair<std::string, Transform*>> listGlobalTransform;
		static int item_current_idx;
#endif // MODE_EDITOR
	};
}