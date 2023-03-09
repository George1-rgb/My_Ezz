#include <iostream>
#include <memory>
#include <imgui/imgui.h>

#include <My_Ezz_Core/Input.hpp>
#include <My_Ezz_Core/Application.hpp>
class My_Ezz_Editor : public My_Ezz::Application
{
	double m_dInitialMousePosX = 0.0;
	double m_dInitialMousePosY = 0.0;

	virtual void on_update() override
	{
		
		glm::vec3 movement_delta{ 0, 0, 0 };
		glm::vec3 rotation_delta{ 0, 0, 0 };

		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_W))
		{
			movement_delta.x += 0.05f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_S))
		{
			movement_delta.x -= 0.05f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_A))
		{
			movement_delta.y -= 0.05f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_D))
		{
			movement_delta.y += 0.05f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_E))
		{
			movement_delta.z += 0.05f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_Q))
		{
			movement_delta.z -= 0.05f;
		}

		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_UP))
		{
			rotation_delta.y -= 0.5f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_DOWN))
		{
			rotation_delta.y += 0.5f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_RIGHT))
		{
			rotation_delta.z -= 0.5f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_LEFT))
		{
			rotation_delta.z += 0.5f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_P))
		{
			rotation_delta.x += 0.5f;
		}
		if (My_Ezz::Input::IsKeyPressed(My_Ezz::KeyCode::KEY_O))
		{
			rotation_delta.x -= 0.5f;
		}

		if (My_Ezz::Input::IsMouseButtonPressed(My_Ezz::MouseButton::MOUSE_BUTTON_RIGHT))
		{
			glm::vec2 currentCursorPosition = GetCurrentCursorPosition();
			if (My_Ezz::Input::IsMouseButtonPressed(My_Ezz::MouseButton::MOUSE_BUTTON_LEFT))
			{
				camera.moveRight((currentCursorPosition.x - m_dInitialMousePosX) / 100.0f);
				camera.moveUp((m_dInitialMousePosY - currentCursorPosition.y) / 100.0f);
			}
			else
			{
				rotation_delta.z += (m_dInitialMousePosX - currentCursorPosition.x) / 5.0f;
				rotation_delta.y -= (m_dInitialMousePosY - currentCursorPosition.y) / 5.0f;
				
			}
			m_dInitialMousePosX = currentCursorPosition.x;
			m_dInitialMousePosY = currentCursorPosition.y;
		}

		camera.add_movement_and_rotation(movement_delta, rotation_delta);
	}

	virtual void OnMouseButtonEvent(const My_Ezz::MouseButton mouseButton, const double x_pos, const double y_pos, const bool bPressed) override
	{
		m_dInitialMousePosX = x_pos;
		m_dInitialMousePosY = y_pos;
	}

	virtual void on_UIDraw() override
	{
		cameraPosition[0] = camera.getCameraPosition().x;
		cameraPosition[1] = camera.getCameraPosition().y;
		cameraPosition[2] = camera.getCameraPosition().z;

		cameraRotation[0] = camera.getCameraRotation().x;
		cameraRotation[1] = camera.getCameraRotation().y;
		cameraRotation[2] = camera.getCameraRotation().z;

		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("Camera position", cameraPosition, -10.0f, 10.0f))
		{
			camera.setPosition(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]));
		}
		if (ImGui::SliderFloat3("Camera rotation", cameraRotation, 0.0f, 360.0f))
		{
			camera.setRotation(glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2]));
		}
		ImGui::Checkbox("Perspective camera", &perspectiveCamera);
		ImGui::End();
	}

	int frame = 0;
};
int main()
{
	auto pMy_Ezz_Editor = std::make_unique<My_Ezz_Editor>();

	int returnCode = pMy_Ezz_Editor->start(1024, 768, "My_Ezz Editor");

	std::cin.get();
	return returnCode;
}