#include "My_Ezz_Core/Input.hpp"

using namespace My_Ezz;

bool Input::m_keysPressed[static_cast<size_t>(KeyCode::KEY_LAST)] = {};

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