

#include "My_Ezz_Core/Application.hpp"

#include "My_Ezz_Logger/Log.hpp"
#include "My_Ezz_Core/Window.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Objects/Image/Texture2D.hpp"
#include "My_Ezz_Core/Camera.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Modules/UIModule.hpp"
#include "My_Ezz_Core/Input.hpp"


#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "My_Ezz_Core/ResourceManager.hpp"
#include "My_Ezz_Multimedia/AudioBase.hpp"
#include "My_Ezz_Multimedia/Multimedia.hpp"
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"
#include "My_Ezz_Core/Objects/SkyBox.hpp"
#include <chrono>
#include <thread>

using namespace My_Ezz;

float m_backgroundColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };

Application::Application()
{
    LOG_INFO("Starting Application");
}

Application::~Application()
{
    LOG_INFO("Closing Application");
}

void Application::Update()
{
	//----------------------------------------//
	Renderer_OpenGL::setClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
	Renderer_OpenGL::clear();
	
	UpdateScene();

	UIModule::onWindowUpdateBegin();
	on_UIDraw();
	UIModule::onWindowUpdateDraw();

	//-------------------------------------------------//
	m_window->on_update();
	on_update();
}


void Application::UpdateScene()
{
	UpdateSkyBoxShadersParams();
	m_pSkyBox->Draw(m_pSkyBoxShaderProgram);

	UpdateMainShadersParams();
	for (auto& pDrawObj : m_vDrawingObjects)
		pDrawObj->Draw(m_pMainShaderProgram);
}


int Application::start(const char* title, bool bAutoSize, unsigned int widnow_width, unsigned int widnow_height)
{

    m_window = std::make_unique<Window>(title, bAutoSize, widnow_width, widnow_height);
    camera.SetViewportSize(static_cast<float>(m_window->get_width()), static_cast<float>(m_window->get_height()));
	camera.setPosition(glm::vec3(-5.0f, 0.f, 0.f));
	if (!InitShaders())
		return -1;
	InitCallbacks();
    Multimedia::InitSoundContext();
	
	std::shared_ptr<Object> pTempObj = ResourceManager::loadObject("test", "res/objects/plane.obj");
	std::string strTest = pTempObj->Serialize();
	if (pTempObj)
		pTempObj->SetPosition(glm::vec3(5.f, 0.f, 0.f));
		m_vDrawingObjects.push_back(pTempObj);


	std::shared_ptr<Texture2D> pSkyTexture = ResourceManager::loadTexture("skyBox", "res/textures/pngegg.png");
	m_pSkyBox = std::make_shared<SkyBox>(100, pSkyTexture);
    Renderer_OpenGL::EnableDepthTesting();

    while (!m_bCloseWindow)
    {
		Update();
    }
    m_window = nullptr;

    return 0;
}

glm::vec2 Application::GetCurrentCursorPosition() const
{
   return m_window->GetCurrentCursorPosition();
}

void Application::InitCallbacks()
{
	m_eventDispatcher.addEventListener<EventMouseMoved>(
		[](EventMouseMoved& event)
		{
			//LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
		});

	m_eventDispatcher.addEventListener<EventWindowResize>(
		[&](EventWindowResize& event)
		{
			//LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
			camera.SetViewportSize(event.width, event.height);
			Update();
		});

	m_eventDispatcher.addEventListener<EventWindowClose>(
		[&](EventWindowClose& event)
		{
			LOG_INFO("[WindowClose]");
			close();
		});

	m_eventDispatcher.addEventListener<EventKeyPressed>(
		[&](EventKeyPressed& event)
		{
			Input::PressKey(event.key_Code);
		});

	m_eventDispatcher.addEventListener<EventKeyReleased>(
		[&](EventKeyReleased& event)
		{
			Input::ReleaseKey(event.key_Code);
		});


	m_eventDispatcher.addEventListener<EventMouseButtonPressed>(
		[&](EventMouseButtonPressed& event)
		{
			Input::PressMouseButton(event.key_Code);
			OnMouseButtonEvent(event.key_Code, event.x_pos, event.y_pos, true);
		});

	m_eventDispatcher.addEventListener<EventMouseButtonReleased>(
		[&](EventMouseButtonReleased& event)
		{
			Input::ReleaseMouseButton(event.key_Code);
			OnMouseButtonEvent(event.key_Code, event.x_pos, event.y_pos, false);
		});

	m_window->set_event_callback(
		[&](BaseEvent& event)
		{
			m_eventDispatcher.dispatch(event);
		});
}

bool Application::InitShaders()
{
	ResourceManager::loadShaders("main_shader", "res/shaders/vshader.vert", "res/shaders/fshader.frag");
	m_pMainShaderProgram = ResourceManager::getShaderProgram("main_shader");
	if (!m_pMainShaderProgram->isCompiled())
	{
		LOG_INFO("The shader main_shader is not compiled");
		return false;
	}
	else
		LOG_INFO("The shader main_shader is compiled");

	ResourceManager::loadShaders("sky_box", "res/shaders/skybox.vsh", "res/shaders/skybox.fsh");
	m_pSkyBoxShaderProgram = ResourceManager::getShaderProgram("sky_box");
	if (!m_pSkyBoxShaderProgram->isCompiled())
	{
		LOG_INFO("The shader skybox is not compiled");
		return false;
	}
	else
		LOG_INFO("The shader skybox is compiled");

	return true;
}

void Application::UpdateMainShadersParams()
{
	m_pMainShaderProgram->bind();
	m_pMainShaderProgram->setUniformValue("u_projectionMatrix", camera.getProjectionMatrix());
	m_pMainShaderProgram->setUniformValue("u_viewMatrix", camera.getViewMatrix());
	m_pMainShaderProgram->setUniformValue("u_shadowMap", GL_TEXTURE4 - GL_TEXTURE0);
	m_pMainShaderProgram->setUniformValue("u_lightDirection", glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

	//TODO:מבתוהוםטעü ס depth רויהונמל
	glm::mat4 projectionLightMatrix = glm::mat4(1.0f);
	projectionLightMatrix = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);
	m_pMainShaderProgram->setUniformValue("u_projectionLightMatrix", projectionLightMatrix);


	glm::mat4 shadowLightMatrix = glm::mat4(1.0f);
	shadowLightMatrix = glm::rotate(shadowLightMatrix, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	shadowLightMatrix = glm::rotate(shadowLightMatrix, 40.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	m_pMainShaderProgram->setUniformValue("u_shadowLightMatrix", shadowLightMatrix);


	glm::mat4 lightMatrix = glm::mat4(1.0f);
	lightMatrix = glm::rotate(lightMatrix, -40.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	lightMatrix = glm::rotate(lightMatrix, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	m_pMainShaderProgram->setUniformValue("u_lightMatrix", lightMatrix);
	m_pMainShaderProgram->setUniformValue("u_lightPower", 10.0f);
}

void Application::UpdateSkyBoxShadersParams()
{
	m_pSkyBoxShaderProgram->bind();
	m_pSkyBoxShaderProgram->setUniformValue("u_projectionMatrix", camera.getProjectionMatrix());
	m_pSkyBoxShaderProgram->setUniformValue("u_viewMatrix", camera.getViewMatrix());
}
