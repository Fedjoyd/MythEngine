#include "ECS/Camera.h"

#include "Core/System/CameraManager.h"

#include "Core/System/Input.h"
#include "Core/System/Time.h"
#include "maths/Maths.hpp"

ECS::Camera::Camera(const bool p_isOrthographique) :
	m_isOrthographique(p_isOrthographique),
	m_isSelected(false),
	Position(0.f, 0.f, 5.f),
	Pitch(0.f),
	Yaw(0.f)
{
}

ECS::Camera::~Camera()
{
	if (Core::System::CameraManager::GetInstance().GetCamera() == this)
		Core::System::CameraManager::GetInstance().SetCamera(nullptr, false);
}

Mat4x4 ECS::Camera::GetViewMatrix() const
{
	return Mat4x4::ViewRad(Pitch, Yaw, Position);
}

void ECS::Camera::Update(Core::GameObject* p_gameObject)
{
	float CosAzimuth = Maths::Cos(Yaw);
	float SinAzimuth = Maths::Sin(Yaw);
	float CosInclination = Maths::Cos(Pitch);
	float SinInclination = Maths::Sin(Pitch);

	float Speed = 4.f;
	if (Core::System::Input::IsKeyDown(CAMERA_SPEED_INPUT_NAME))
		Speed *= 10.f;
	float FrameSpeed = (float)(Speed * Core::System::Time::DeltaTime());

	float ForwardVelocity = 0.f;
	float StrafeVelocity = 0.f;

	if (Core::System::Input::IsKeyDown(CAMERA_FRONT_INPUT_NAME)) ForwardVelocity += -FrameSpeed;
	if (Core::System::Input::IsKeyDown(CAMERA_BACK_INPUT_NAME)) ForwardVelocity += +FrameSpeed;

	if (Core::System::Input::IsKeyDown(CAMERA_LEFT_INPUT_NAME)) StrafeVelocity += -FrameSpeed;
	if (Core::System::Input::IsKeyDown(CAMERA_RIGHT_INPUT_NAME)) StrafeVelocity += +FrameSpeed;

	if (Core::System::Input::IsKeyDown(CAMERA_UP_INPUT_NAME)) Position.m_y += FrameSpeed;
	if (Core::System::Input::IsKeyDown(CAMERA_DOWN_INPUT_NAME)) Position.m_y -= FrameSpeed;

	// Forward movement
	Position.m_z += CosAzimuth * CosInclination * ForwardVelocity;
	Position.m_x += -SinAzimuth * CosInclination * ForwardVelocity;
	Position.m_y += -SinInclination * ForwardVelocity;

	// Strafe movement
	Position.m_z += SinAzimuth * StrafeVelocity;
	Position.m_x += CosAzimuth * StrafeVelocity;

	Core::System::InputStatus LookInputStatus = Core::System::Input::GetKeyInputStatus(CAMERA_LOOK_INPUT_NAME);
	if (LookInputStatus.IsPressed) Core::System::Input::GetInstance().SetMouseLock(true);
	if (LookInputStatus.IsReleased) Core::System::Input::GetInstance().SetMouseLock(false);
	if (LookInputStatus.IsDown)
	{
		Vec2 DeltaMouse = Core::System::Input::GetMouseDelta();

		// Yaw
		Yaw += DeltaMouse.m_x * CAM_MOUSE_SENSITIVITY_X;
		Yaw = Maths::Mod((Yaw + TAU) + PI, TAU) - PI; // Loop around -180,180

		// Pitch
		Pitch += DeltaMouse.m_y * CAM_MOUSE_SENSITIVITY_Y;
		Pitch = Maths::Clamp(Pitch, -(PI/2), PI/2); // Limit rotation to -90,90 range
	}
}

#ifdef MODE_EDITOR
void ECS::Camera::EditorUpdate(Core::GameObject* p_gameObject)
{
	Update(p_gameObject);
}

#include "imgui.h"

void ECS::Camera::ShowEditorControl(const unsigned int p_indexComponent)
{
	ImGui::SameLine();

	if (m_isSelected)
	{
		if (ImGui::Button(("Deselect##camera" + std::to_string(p_indexComponent)).c_str()))
		{
			Core::System::CameraManager::GetInstance().SetCamera(nullptr);
			SetSelected(false);
		}
	}
	else
	{
		if (ImGui::Button(("Select##camera" + std::to_string(p_indexComponent)).c_str()))
			Core::System::CameraManager::GetInstance().SetCamera(this);
	}

	ImGui::Spacing();

	ImGui::Checkbox(("Is ortographique##camera" + std::to_string(p_indexComponent)).c_str(), &m_isOrthographique);

	ImGui::DragFloat3(("Position##camera" + std::to_string(p_indexComponent)).c_str(), (float*)&Position);
	ImGui::DragFloat(("Pitch##camera" + std::to_string(p_indexComponent)).c_str(), &Pitch);
	ImGui::DragFloat(("Yaw##camera" + std::to_string(p_indexComponent)).c_str(), &Yaw);
}
#endif // MODE_EDITOR
