#pragma once
#include <memory>
#include "My_Ezz_Core/Event.hpp"
#include "My_Ezz_Core/Camera.hpp"

namespace My_Ezz
{
	class Application
	{
	public:
		Application();
		virtual ~Application();
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(unsigned int widnow_width, unsigned int widnow_height, const char* title);
		void close() { m_bCloseWindow = true; }
		virtual void on_update() {}

		virtual void on_UIDraw() {}

		virtual void OnMouseButtonEvent(const MouseButton mouseButton, const double x_pos, const double y_pos, const bool bPressed) {}

		glm::vec2 GetCurrentCursorPosition() const;

		float cameraPosition[3] = { 0.0f, 0.0f, 1.1f };
		float cameraRotation[3] = { 0.0f, 0.0f, 0.0f };
		float cameraFieldOfView = 60.f;
		float cameraNearPlane = 0.1f;
		float cameraFarPlane = 100.f;
		bool perspectiveCamera = true;
		Camera camera{glm::vec3(-5.0f, 0.0f, 0.0f)};

	private:
		void draw();

		std::unique_ptr<class Window> m_window;
		EventDispatcher m_eventDispatcher;
		bool m_bCloseWindow = false;

	};
}