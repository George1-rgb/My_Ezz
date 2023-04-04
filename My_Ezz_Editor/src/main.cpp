#include <iostream>
#include <memory>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

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

	void SetUpDockspaceMenu()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoWindowMenuButton;
		static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New Scene", NULL))
				{

				}
				if (ImGui::MenuItem("Open Scene", NULL))
				{

				}
				if (ImGui::MenuItem("Save Scene", NULL))
				{

				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", NULL))
				{
					close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	virtual void OnMouseButtonEvent(const My_Ezz::MouseButton mouseButton, const double x_pos, const double y_pos, const bool bPressed) override
	{
		m_dInitialMousePosX = x_pos;
		m_dInitialMousePosY = y_pos;
	}

	virtual void on_UIDraw() override
	{
		SetUpDockspaceMenu();
		cameraPosition[0] = camera.getPosition().x;
		cameraPosition[1] = camera.getPosition().y;
		cameraPosition[2] = camera.getPosition().z;

		cameraRotation[0] = camera.getRotation().x;
		cameraRotation[1] = camera.getRotation().y;
		cameraRotation[2] = camera.getRotation().z;

		cameraFieldOfView = camera.GetFieldOfView();
		cameraNearPlane = camera.GetNearClipPlane();
		cameraFarPlane = camera.GetFarClipPlane();


		ImGui::Begin("Editor");
		if (ImGui::SliderFloat3("Camera position", cameraPosition, -10.0f, 10.0f))
		{
			camera.setPosition(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]));
		}
		if (ImGui::SliderFloat3("Camera rotation", cameraRotation, 0.0f, 360.0f))
		{
			camera.setRotation(glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2]));
		}
		if (ImGui::SliderFloat("Camera FOV", &cameraFieldOfView, 1.0f, 120.0f))
		{
			camera.SetFieldOfView(cameraFieldOfView);
		}
		if (ImGui::SliderFloat("Camera near plane", &cameraNearPlane, 0.1f, 10.0f))
		{
			camera.SetNearClipPlane(cameraNearPlane);
		}
		if (ImGui::SliderFloat("Camera far plane", &cameraFarPlane, 1.0f, 100.0f))
		{
			camera.SetFarClipPlane(cameraFarPlane);
		}
		if (ImGui::Checkbox("Perspective camera", &perspectiveCamera))
		{
			camera.setProjectionMode(perspectiveCamera ? My_Ezz::Camera::ProjectionMode::Perspective : My_Ezz::Camera::ProjectionMode::Orthographic);
		}
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