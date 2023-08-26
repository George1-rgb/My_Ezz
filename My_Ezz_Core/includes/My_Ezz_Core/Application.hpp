#pragma once
#include <memory>
#include "My_Ezz_Core/Event.hpp"
#include "My_Ezz_Core/Camera.hpp"
#include "My_Ezz_Core/Interfaces.hpp"

#include "My_Ezz_Core/Objects/Object.hpp"
#include "My_Ezz_Core/Objects/LightBase.hpp"
#include "My_Ezz_Core/Interfaces.hpp"
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

		Camera camera{glm::vec3(-5.0f, 0.0f, 0.0f)};
		float fLightSourcePosition[3] = { 0.0f, 0.0f, 0.0f };
		float fLightSourceColor[3] = { 1.0f, 1.0f, 1.0f };
		float fAmbientFactor = 0.1f;
		float fDiffuseFactor = 1.f;
		float fSpecularFactor = 0.5f;
		float fShininess = 32.f;
	private:
		void draw();
		std::unique_ptr<class Window> m_window;
		EventDispatcher m_eventDispatcher;
		bool m_bCloseWindow = false;
		std::vector<std::shared_ptr<Object>> m_vDrawingObjects;
		std::shared_ptr<LightBase> m_pLightObj;
	};
}