#include "Core/System/Input.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_stdlib.h"

Core::System::Input Core::System::Input::singleton;

Core::System::Input::Input()
{
}

Core::System::Input& Core::System::Input::GetInstance()
{
	return singleton;
}

void Core::System::Input::Update(GLFWwindow* window)
{
	if (window == nullptr)
		return;

	int KeyStatus = 0;

	for (unsigned int i = 0; i < m_keyboardMouseInput.size(); i++)
	{
		if (m_keyboardMouseInput[i].KeyCodes.empty())
		{
			m_keyboardMouseInput.erase(m_keyboardMouseInput.begin() + i);
			i--;
			continue;
		}

		KeyboardMouseInputData& currentInputdata = m_keyboardMouseInput[i];

		currentInputdata.PreviousDown = currentInputdata.IsDown;
		currentInputdata.IsRepeated = false;
		currentInputdata.IsDown = false;

		for (const std::pair<bool, int>& currentKeyCode : currentInputdata.KeyCodes)
		{
			if (currentKeyCode.first)
				KeyStatus = glfwGetMouseButton(window, currentKeyCode.second);
			else
				KeyStatus = glfwGetKey(window, currentKeyCode.second);

			currentInputdata.IsRepeated = currentInputdata.IsRepeated || (KeyStatus == GLFW_REPEAT);
			currentInputdata.IsDown = currentInputdata.IsDown || ((KeyStatus == GLFW_PRESS) || (KeyStatus == GLFW_REPEAT));
		}
	}
}

Core::System::KeyboardMouseInputData* Core::System::Input::GetKeyInput(const std::string& p_inputName)
{
	for (unsigned int i = 0; i < m_keyboardMouseInput.size(); i++)
		if (m_keyboardMouseInput[i].Name == p_inputName)
			return &(m_keyboardMouseInput[i]);

	return nullptr;
}

void Core::System::Input::AddKeyInput(const std::string& p_inputName, int p_keycode, bool p_isMouse)
{
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
	{
		m_keyboardMouseInput.emplace_back();
		inputData = &m_keyboardMouseInput.back();
		inputData->Name = p_inputName;
	}

	inputData->KeyCodes.push_back({ p_isMouse , p_keycode });
}

void Core::System::Input::RemKeyInput(const std::string& p_inputName, int p_keycode, bool p_isMouse)
{
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
		return;

	for (unsigned int i = 0; i < inputData->KeyCodes.size(); i++)
	{
		if (inputData->KeyCodes[i].first == p_isMouse)
			if (inputData->KeyCodes[i].second == p_keycode)
			{
				inputData->KeyCodes.erase(inputData->KeyCodes.begin() + i);
				i--;
			}
	}
}

Core::System::InputStatus Core::System::Input::GetKeyInputStatus(const std::string& p_inputName)
{
	InputStatus to_return = { false, false, false, false };
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
		return to_return;

	to_return.IsDown = inputData->IsDown;
	to_return.IsPressed = (inputData->IsDown && !inputData->PreviousDown);
	to_return.IsReleased = (!inputData->IsDown && inputData->PreviousDown);
	to_return.IsRepeated = inputData->IsRepeated;

	return to_return;
}

bool Core::System::Input::IsKeyDown(const std::string& p_inputName)
{
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
		return false;

	return inputData->IsDown;
}

bool Core::System::Input::IsKeyPressed(const std::string& p_inputName)
{
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
		return false;

	return (inputData->IsDown && !inputData->PreviousDown);
}

bool Core::System::Input::IsKeyReleased(const std::string& p_inputName)
{
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
		return false;

	return (!inputData->IsDown && inputData->PreviousDown);
}

bool Core::System::Input::IsKeyRepeated(const std::string& p_inputName)
{
	KeyboardMouseInputData* inputData = singleton.GetKeyInput(p_inputName);

	if (inputData == nullptr)
		return false;

	return inputData->IsRepeated;
}

#ifdef MODE_EDITOR
void Core::System::keyboard_button_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// KEY CALLBACK TO BIND INPUTS (Replace the callback set by ImGui)

	lastKeyPressedIsMouse = false;

	if (action == GLFW_PRESS)
		lastKeyPressed = key;
	else
		lastKeyPressed = GLFW_KEY_UNKNOWN;

	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void Core::System::mouse_button_callback(GLFWwindow* window, int key, int action, int mods)
{
	// MOUSE KEY CALLBACK TO BIND INPUTS (Replace the callback set by ImGui)

	lastKeyPressedIsMouse = true;

	if (action == GLFW_PRESS)
		lastKeyPressed = key;
	else
		lastKeyPressed = GLFW_KEY_UNKNOWN;

	ImGui_ImplGlfw_MouseButtonCallback(window, key, action, mods);
}

void Core::System::SetKeyCallback(GLFWwindow* window)
{
	glfwSetKeyCallback(window, keyboard_button_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

std::string getKeyName(int key, const bool isMouse)
{
	// Some inputs have no name with glfwGetKeyName()

	if (isMouse)
	{
		if (key == GLFW_MOUSE_BUTTON_RIGHT)
			return "MOUSE RIGHT";
		if (key == GLFW_MOUSE_BUTTON_LEFT)
			return "MOUSE LEFT";
		if (key == GLFW_MOUSE_BUTTON_MIDDLE)
			return "MOUSE MIDDLE";
		if (key == GLFW_MOUSE_BUTTON_LAST)
			return "MOUSE LAST";

		return std::to_string(key);
	}

	if (key == GLFW_KEY_UNKNOWN)
		return "UNSET";

	else if (key == 256)
		return "ESCAPE";

	else if (key == 32)
		return "SPACE";

	else if (key == 262)
		return "RIGHT";

	else if (key == 263)
		return "LEFT";

	else if (key == 264)
		return "DOWN";

	else if (key == 265)
		return "UP";

	else if (key >= 290 && key < 301)
		return "F" + std::to_string(key - 289);

	else if (key == 341)
		return "Left-CTRL";

	else if (key == 340)
		return "Left-SHIFT";

	else
		return glfwGetKeyName(key, glfwGetKeyScancode(key));
}

void Core::System::Input::ShowEditorWindow(bool* p_opened)
{
	if (p_opened != nullptr)
		if (!(*p_opened))
			return;

	ImGui::Begin("Input Editor", p_opened);

	std::vector<std::pair<std::string, std::pair<bool, int>>> toRemoveKey;

	for (unsigned int indexInput = 0; indexInput < m_keyboardMouseInput.size(); indexInput++)
		if (ImGui::TreeNode((m_keyboardMouseInput[indexInput].Name + "##indexInput" + std::to_string(indexInput)).c_str()))
		{
			KeyboardMouseInputData& currentData = m_keyboardMouseInput[indexInput];

			ImGui::Text(("CURRENT STATE : " + std::string(currentData.IsDown ? "DOWN" : "UP")).c_str());
			ImGui::Separator();

			for (unsigned int indexkey = 0; indexkey < currentData.KeyCodes.size(); indexkey++)
			{
				std::pair<bool, int>& currentKey = currentData.KeyCodes[indexkey];

				ImGui::Text(getKeyName(currentKey.second, currentKey.first).c_str());
				ImGui::SameLine();
				if (ImGui::Button(("Set##indexkey" + std::to_string(indexkey)).c_str()))
					SetInputKeycode(currentKey);
				ImGui::SameLine();
				if (ImGui::Button(("Delete##indexkey" + std::to_string(indexkey)).c_str()))
					toRemoveKey.push_back({ currentData.Name , {currentKey.first , currentKey.second} });
			}

			if (ImGui::Button(("Add Key##indexInput" + std::to_string(indexInput)).c_str()))
				AddKeyInput(currentData.Name, GLFW_KEY_UNKNOWN);

			ImGui::TreePop();
			ImGui::Separator();
		}

	static std::string newInputName = "";
	ImGui::InputText("##InputName", &newInputName);
	ImGui::SameLine();
	if (ImGui::Button("Add Input##Input"))
		AddKeyInput(newInputName, GLFW_KEY_UNKNOWN);

	for (const std::pair<std::string, std::pair<bool, int>>& currentToRem : toRemoveKey)
		RemKeyInput(currentToRem.first, currentToRem.second.second, currentToRem.second.first);

	ImGui::End();
}

void Core::System::Input::SetInputKeycode(std::pair<bool, int>& pairToUpdate)
{
	lastKeyPressed = GLFW_KEY_UNKNOWN;

	// Wait before listen input to be sure that user put the wanted key
	int wait = 10000;
	while (wait > 0 || lastKeyPressed == GLFW_KEY_UNKNOWN)
	{
		glfwPollEvents();
		wait -= wait < -1 ? 0 : 1;
	}

	// Forbid escape binding
	if (lastKeyPressed == GLFW_KEY_ESCAPE)
	{
		pairToUpdate.first = false;
		pairToUpdate.second = GLFW_KEY_UNKNOWN;
		return;
	}

	// Forbid timeout
	if (lastKeyPressed == -1) return;

	pairToUpdate.first = lastKeyPressedIsMouse;
	pairToUpdate.second = lastKeyPressed;
}
#endif // MODE_EDITOR
