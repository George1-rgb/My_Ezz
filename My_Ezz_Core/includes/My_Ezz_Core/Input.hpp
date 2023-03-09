#pragma once

#include "Keys.hpp"

namespace My_Ezz
{
	class Input
	{
	public:
		static bool IsKeyPressed(const KeyCode key_code);
		static void PressKey(const KeyCode key_code);
		static void ReleaseKey(const KeyCode key_code);

		static bool IsMouseButtonPressed(const MouseButton eMouseButton);
		static void PressMouseButton(const MouseButton eMouseButton);
		static void ReleaseMouseButton(const MouseButton eMouseButton);
	private:
		static bool m_keysPressed[];
		static bool m_mouseKeysPressed[];
	};
}