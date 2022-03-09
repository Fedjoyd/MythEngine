#pragma once

#include "define.h"

#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#include "maths/Vec2.hpp"

namespace Core
{
	namespace System
	{
		struct KeyboardMouseInputData
		{
			std::string Name = "";
			std::vector<std::pair<bool, int>> KeyCodes = std::vector<std::pair<bool, int>>(); // { isMouse, keycode }
			bool IsDown = false;
			bool PreviousDown = false;
			bool IsRepeated = false;
		};

		struct InputStatus
		{
			bool IsDown;
			bool IsPressed;
			bool IsReleased;
			bool IsRepeated;
		};

#ifdef MODE_EDITOR
		static int lastKeyPressed = GLFW_KEY_UNKNOWN;
		static bool lastKeyPressedIsMouse = false;

		void keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void mouse_button_callback(GLFWwindow* window, int key, int action, int mods);
		void SetKeyCallback(GLFWwindow* window);
#endif // MODE_EDITOR

		class Input
		{
		public:
			~Input() {}

			static Input& GetInstance();

			void Update(GLFWwindow* window);
			bool HasKeyInput(const std::string& p_inputName);
			KeyboardMouseInputData* GetKeyInput(const std::string& p_inputName);
			void AddKeyInput(const std::string& p_inputName, int p_keycode, bool p_isMouse = false);
			void RemKeyInput(const std::string& p_inputName, int p_keycode, bool p_isMouse = false);
			bool IsMouseLocked() const { return m_lockMouse; }
			void SetMouseLock(const bool p_newLock = true) { m_lockMouse = p_newLock; }

			static InputStatus GetKeyInputStatus(const std::string& p_inputName);
			static bool IsKeyDown(const std::string& p_inputName);
			static bool IsKeyPressed(const std::string& p_inputName);
			static bool IsKeyReleased(const std::string& p_inputName);
			static bool IsKeyRepeated(const std::string& p_inputName);

			static Vec2 GetMousePosition() { return Vec2((float)singleton.m_xMousePosition, (float)singleton.m_yMousePosition); }
			static Vec2 GetMouseDelta();

#ifdef MODE_EDITOR
			void ShowEditorWindow(bool* p_opened = nullptr);
#endif // MODE_EDITOR

		private:
			Input();
			static Input singleton;

#ifdef MODE_EDITOR
			void SetInputKeycode(std::pair<bool, int>& pairToUpdate);
#endif // MODE_EDITOR

			std::vector<KeyboardMouseInputData> m_keyboardMouseInput;

			double m_xMousePosition;
			double m_yMousePosition;
			double m_xOldMousePosition;
			double m_yOldMousePosition;

			bool m_lockMouse;
		};
	}
}