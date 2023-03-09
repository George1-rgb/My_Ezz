#include "My_Ezz_Core/Input.hpp"

using namespace My_Ezz;

bool Input::m_keysPressed[static_cast<size_t>(KeyCode::KEY_LAST) + 1] = {};
bool Input::m_mouseKeysPressed[static_cast<size_t>(MouseButton::MOUSE_BUTTON_LAST) + 1];

bool Input::IsKeyPressed(const KeyCode key_code)
{
	return m_keysPressed[static_cast<size_t>(key_code)];
}

void Input::PressKey(const KeyCode key_code)
{
	m_keysPressed[static_cast<size_t>(key_code)] = true;
}

void Input::ReleaseKey(const KeyCode key_code)
{
	m_keysPressed[static_cast<size_t>(key_code)] = false;
}

bool Input::IsMouseButtonPressed(const MouseButton eMouseButton)
{
	return m_mouseKeysPressed[static_cast<size_t>(eMouseButton)];
}

void Input::PressMouseButton(const MouseButton eMouseButton)
{
	m_mouseKeysPressed[static_cast<size_t>(eMouseButton)] = true;
}

void Input::ReleaseMouseButton(const MouseButton eMouseButton)
{
	m_mouseKeysPressed[static_cast<size_t>(eMouseButton)] = false;
}