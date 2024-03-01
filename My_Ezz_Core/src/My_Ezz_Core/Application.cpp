

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
	Renderer_OpenGL::EnableDepthTesting();

	UpdateSkyBoxShadersParams();
	m_pSkyBox->Draw(m_pSkyBoxShaderProgram);

		UpdateMainShadersParams();
		for (auto& pDrawObj : m_vDrawingObjects)
		{
			pDrawObj->Rotate(glm::vec3(0.1, 0.1, 0.1));
			pDrawObj->Draw(m_pMainShaderProgram, m_mSelectedObjets.find(pDrawObj->GetID()) != m_mSelectedObjets.end());
		}
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

	if (pTempObj)
		pTempObj->SetPosition(glm::vec3(5.f, -5.f, 0.f));
		m_vDrawingObjects.push_back(pTempObj);

	std::shared_ptr<Object> pTempObj_2 = ResourceManager::loadObject("test_2", "res/objects/plane.obj");
	if (pTempObj_2)
		pTempObj_2->SetPosition(glm::vec3(5.f, 5.f, 0.f));
	m_vDrawingObjects.push_back(pTempObj_2);

	std::shared_ptr<Texture2D> pSkyTexture = ResourceManager::loadTexture("skyBox", "res/textures/skybox.png");
	m_pSkyBox = std::make_shared<SkyBox>(50, pSkyTexture);
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

	ResourceManager::loadShaders("select", "res/shaders/select.vert", "res/shaders/select.frag");
	m_pSelectShaderProgram = ResourceManager::getShaderProgram("select");
	if (!m_pSelectShaderProgram->isCompiled())
	{
		LOG_INFO("The shader select is not compiled");
		return false;
	}
	else
		LOG_INFO("The shader select is compiled");

	//ResourceManager::loadShaders("selectedFrame", "res/shaders/selected_farme.vert", "res/shaders/selected_farme.frag");
	//m_pSelectedFrameShaderProgram = ResourceManager::getShaderProgram("selectedFrame");
	//if (!m_pSelectedFrameShaderProgram->isCompiled())
	//{
	//	LOG_INFO("The shader selectedFrame is not compiled");
	//	return false;
	//}
	//else
	//	LOG_INFO("The shader selectedFrame is compiled");


	return true;
}

void Application::UpdateMainShadersParams()
{
	m_pMainShaderProgram->bind();
	m_pMainShaderProgram->setUniformValue("u_projectionMatrix", camera.getProjectionMatrix());
	m_pMainShaderProgram->setUniformValue("u_viewMatrix", camera.getViewMatrix());
	m_pMainShaderProgram->setUniformValue("u_shadowMap", GL_TEXTURE4 - GL_TEXTURE0);
	m_pMainShaderProgram->setUniformValue("u_lightDirection", glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	m_pMainShaderProgram->setUniformValue("u_borderWidth", 0.0001);
	m_pMainShaderProgram->setUniformValue("u_aspectRatio", camera.GetAspectRatio());
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

void Application::OnMouseButtonEvent(const MouseButton mouseButton, const double x_pos, const double y_pos, const bool bPressed)
{
	switch (mouseButton)
	{
	case MouseButton::MOUSE_BUTTON_LEFT:
	{
		if (!bPressed)
			return;
		std::shared_ptr<Object> pPickedObject = GetPickedObject(x_pos, y_pos);

		if (pPickedObject)
		{
			auto pIter = m_mSelectedObjets.find(pPickedObject->GetID());
			if (pIter == m_mSelectedObjets.end())
				m_mSelectedObjets.insert(std::pair<int, std::shared_ptr<Object>>(pPickedObject->GetID(), pPickedObject));
			else
			{
				m_mSelectedObjets.erase(pIter);
			}
		}
		else
			m_mSelectedObjets.clear();
	}
	break;
	}
}

std::shared_ptr<My_Ezz::Object> Application::GetPickedObject(const int& nX, const int& nY)
{
	Renderer_OpenGL::DisableDepthTesting();
	m_pSelectShaderProgram->bind();
	m_pSelectShaderProgram->setUniformValue("u_projectionMatrix", camera.getProjectionMatrix());
	m_pSelectShaderProgram->setUniformValue("u_viewMatrix", camera.getViewMatrix());
	for (const auto& pObject : m_vDrawingObjects)
	{
		int id = pObject->GetID();
		m_pSelectShaderProgram->setUniformValue("u_code", static_cast<double>(id) / 255.0);
		pObject->Draw(m_pSelectShaderProgram);
	}

	unsigned char data[4];
	glReadPixels(nX, camera.GetVPHeight() - nY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
	int nData = 0;

	nData += data[0];


	auto pObject = std::find_if(m_vDrawingObjects.begin(), m_vDrawingObjects.end(),
		[nData](const std::shared_ptr<Object>& pFindedObject)
		{
			return nData == pFindedObject->GetID();
		});
	if (pObject != m_vDrawingObjects.end())
		return *pObject;
	return nullptr;
}