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
	typedef std::unordered_map<int, std::shared_ptr<Object>> ObjectsMap; //id, object
	class SkyBox;
	class Application
	{
	public:
		Application();
		virtual ~Application();
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(const Application&) = delete;
		Application& operator=(Application&&) = delete;

		virtual int start(const char* title, bool bAutoSize, unsigned int widnow_width = 0, unsigned int widnow_height = 0);
		void close() { m_bCloseWindow = true; }
		virtual void on_update() {}

		virtual void on_UIDraw() {}

		virtual void OnMouseButtonEvent(const MouseButton mouseButton, const double x_pos, const double y_pos, const bool bPressed);

		glm::vec2 GetCurrentCursorPosition() const;

		Camera camera{glm::vec3(-5.0f, 0.0f, 0.0f)};
		float fAngles[3] = { 0.0f, 0.0f, 0.0f };
	protected:
		std::shared_ptr<Object> GetPickedObject(const int& nX, const int& nY);
	private:
		void InitCallbacks();
		bool InitShaders();
		void Update();
		void UpdateScene();
		void UpdateMainShadersParams();
		void UpdateSkyBoxShadersParams();
		std::unique_ptr<class Window> m_window;
		EventDispatcher m_eventDispatcher;
		bool m_bCloseWindow = false;
		std::vector<std::shared_ptr<Object>> m_vDrawingObjects;
		std::shared_ptr<SkyBox> m_pSkyBox;
		std::shared_ptr<LightBase> m_pLightObj;
		std::shared_ptr<ShaderProgram> m_pMainShaderProgram;
		std::shared_ptr<ShaderProgram> m_pSkyBoxShaderProgram;
		std::shared_ptr<ShaderProgram> m_pSelectShaderProgram;
		std::shared_ptr<ShaderProgram> m_pSelectedFrameShaderProgram;

		ObjectsMap m_mSelectedObjets;
	};
}