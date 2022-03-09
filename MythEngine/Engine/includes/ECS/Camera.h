#pragma once

#include "define.h"

#include "Component.h"

#include "maths/Vec3.hpp"
#include "maths/Quaternion.hpp"

namespace ECS
{
	class Camera : public Component
	{
	public:
		Camera(const bool p_isOrthographique = false);
		~Camera();

		const std::type_info& GetComponentType() const override { return typeid(*this); }

		bool IsOrthographique() const { return m_isOrthographique; }
		void SetOrthographique(const bool p_isOrthographique) { m_isOrthographique = p_isOrthographique; }

		bool IsSelected() const { return m_isSelected; }
		void SetSelected(const bool p_isSelected) { m_isSelected = p_isSelected; }

		Vec3  Position;
		float Pitch;
		float Yaw;

		Mat4x4 GetViewMatrix() const;

		void Update(Core::GameObject* p_gameObject) override;
#ifdef MODE_EDITOR
		void EditorUpdate(Core::GameObject* p_gameObject) override;
		void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // MODE_EDITOR

	private:
		bool m_isOrthographique;
		bool m_isSelected;
	};
}