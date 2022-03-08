#include "ECS/Transform.h"

#include "imgui.h"

ECS::Transform::Transform(Transform* p_parent) :
	m_parent(p_parent),
	m_childs(),
	Position(0.f, 0.f, 0.f),
	Rotation(0.f, 0.f, 0.f, 0.f),
	Scale(0.f, 0.f, 0.f),
	m_oldPosition(0.f, 0.f, 0.f),
	m_oldRotation(0.f, 0.f, 0.f, 0.f),
	m_oldScale(0.f, 0.f, 0.f),
	m_localMatrix()
{
	if (m_parent != nullptr)
		m_parent->AddChild(this);
}

ECS::Transform::~Transform()
{
	if (m_parent != nullptr)
		m_parent->RemChild(this);

	for (Transform* child : m_childs)
		child->SetParent(nullptr, false);
}

void ECS::Transform::SetParent(Transform* p_parent, bool unsetParent)
{
	if (m_parent != nullptr && unsetParent)
		m_parent->RemChild(this);

	m_parent = p_parent;

	if (m_parent != nullptr)
		m_parent->AddChild(this);
}

void ECS::Transform::AddChild(Transform* p_child)
{
	m_childs.push_back(p_child);
}

void ECS::Transform::RemChild(Transform* p_child)
{
	for (unsigned int i = 0; i < m_childs.size(); i++)
		if (m_childs[i] == p_child)
		{
			m_childs.erase(m_childs.begin() + i);
			i--;
		}
}

Vec3 ECS::Transform::GetGlobalPosition() const
{
	if (m_parent != nullptr)
	{
		return Quaternion::Vec3RotateByQuat(m_parent->GetGlobalPosition(), m_parent->GetGlobalRotation()) + Position;
	}

	return Position;
}

Quaternion ECS::Transform::GetGlobalRotation() const
{
	if (m_parent != nullptr)
	{
		return Rotation * m_parent->GetGlobalRotation();
	}

	return Rotation;
}

Vec3 ECS::Transform::GetGlobalScale() const
{
	if (m_parent != nullptr)
	{
		return m_parent->GetGlobalScale() * Scale;
	}

	return Scale;
}

Mat4x4 ECS::Transform::GetGlobalMatrix() const
{
	if (m_parent != nullptr)
	{
		return m_parent->GetGlobalMatrix() * m_localMatrix;
	}

	return m_localMatrix;
}

void ECS::Transform::Update(Core::GameObject* p_gameObject)
{
	if (Position == m_oldPosition &&
		Rotation == m_oldRotation &&
		Scale == m_oldScale)
		return;

	m_localMatrix = Mat4x4::Translate(Position) *
		            Rotation.GetRotationMatrix() *
		            Mat4x4::Scale(Scale);

	m_oldPosition = Position;
	m_oldRotation = Rotation;
	m_oldScale = Scale;
}

#ifdef MODE_EDITOR
#include "Core/GameObject.h"
#include "Core/System/Application.h"

void ECS::Transform::EditorUpdate(Core::GameObject* p_gameObject)
{
	if (Position == m_oldPosition &&
		Rotation == m_oldRotation &&
		Scale == m_oldScale)
		return;

	m_localMatrix = Mat4x4::Translate(Position) *
		Rotation.GetRotationMatrix() *
		Mat4x4::Scale(Scale);

	m_oldPosition = Position;
	m_oldRotation = Rotation;
	m_oldScale = Scale;
}

std::vector<std::pair<std::string, ECS::Transform*>> ECS::Transform::listGlobalTransform;
int ECS::Transform::item_current_idx = 0;

void ECS::Transform::ShowEditorControl(const unsigned int p_indexComponent)
{
	if (m_parent != nullptr)
	{
		ImGui::Text("Parent : %p", m_parent);
		ImGui::SameLine();
		if (ImGui::Button(("Remove parent##transform-" + std::to_string(p_indexComponent)).c_str()))
			SetParent(nullptr);
	}
	else
	{
		ImGui::Text("Parent : NONE");
		ImGui::SameLine();
		if (ImGui::Button(("Select parent##transform-" + std::to_string(p_indexComponent)).c_str()))
		{
			listGlobalTransform.clear();

			for (std::unique_ptr<Core::GameObject>& curentObj : Core::System::Application::GetInstance().GetGameObjectManager().GetGameObjects())
			{
				std::vector<Transform*> curListTransf = curentObj->GetAllComponents<Transform>();

				for (Transform* curTransfo : curListTransf)
					listGlobalTransform.push_back({ curentObj->GetName(), curTransfo });
			}

			ImGui::OpenPopup("SelectTranformParent?");
		}

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal("SelectTranformParent?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::BeginListBox("##list transform"))
			{
				for (unsigned int n = 0; n < listGlobalTransform.size(); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable((listGlobalTransform[n].first + " - " + std::to_string(n)).c_str(), is_selected))
						item_current_idx = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				if (listGlobalTransform[item_current_idx].second != this)
					SetParent(listGlobalTransform[item_current_idx].second);
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}

	ImGui::Spacing();

	ImGui::DragFloat3(("position##transform-" + std::to_string(p_indexComponent)).c_str(), (float*)&Position);
	ImGui::DragFloat4(("rotation##transform-" + std::to_string(p_indexComponent)).c_str(), (float*)&Rotation);
	ImGui::DragFloat3(("Scale##transform-" + std::to_string(p_indexComponent)).c_str(), (float*)&Scale);
}
#endif // MODE_EDITOR