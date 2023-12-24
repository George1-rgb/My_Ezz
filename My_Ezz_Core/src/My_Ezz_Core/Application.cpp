

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

#include <chrono>
#include <thread>

using namespace My_Ezz;

std::shared_ptr<ShaderProgram> pShaderProgram;
std::shared_ptr<ShaderProgram> pLightSourceShaderProgram;
std::shared_ptr<VertexBuffer> pCubePositionsVBO;
std::shared_ptr<IndexBuffer> pCubeIndexBuffer;
//std::shared_ptr<Texture2D> pTexture_Smile;
//std::shared_ptr<Texture2D> pTexture_Quads;
std::shared_ptr<VertexArray> pVAO_1;
std::shared_ptr<Object> objPlane;
std::shared_ptr<Object> objSphere;
std::shared_ptr<Object> objCube;
std::shared_ptr<Object> objTest;

float m_backgroundColor[4] = { 0.5f, 0.5f, 0.5f, 0.0f };

std::array<glm::vec3, 4> positionsCubes =
{
    glm::vec3(0.f, -3.f, -3.f),
    glm::vec3(0.f, -3.f, 3.f),
    glm::vec3(0.f, 3.f, -3.f),
    glm::vec3(0.f, 3.f, 3.f)
};

Application::Application()
{
    LOG_INFO("Starting Application");
}

Application::~Application()
{
    LOG_INFO("Closing Application");
}


void Application::timer() {

	// Infinite while loop.
    std::chrono::milliseconds timespan(20); // or whatever
	while (true) {
        fAngles[0] += 1.0f;
        fAngles[1] += 1.0f;
        fAngles[2] += 1.0f;

        if (fAngles[0] >= 360.0f)
            fAngles[0] = 0;
		if (fAngles[1] >= 360.0f)
			fAngles[1] = 0;
		if (fAngles[2] >= 360.0f)
			fAngles[2] = 0;
		std::this_thread::sleep_for(timespan);
	}
}

void Application::draw()
{
	//----------------------------------------//
	Renderer_OpenGL::setClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
	Renderer_OpenGL::clear();

	//////light source
	//{
	//	pLightSourceShaderProgram->bind();
	//	m_pLightObj->SetPosition(glm::vec3(fLightSourcePosition[0], fLightSourcePosition[1], fLightSourcePosition[2]));
	//	m_pLightObj->SetShaderMatrix(camera.getProjectionMatrix(), camera.getViewMatrix());
	//	m_pLightObj->SetColor(glm::vec3(fLightSourceColor[0], fLightSourceColor[1], fLightSourceColor[2]));
	//	m_pLightObj->SetScale(0.5);
	//	m_pLightObj->Draw(pLightSourceShaderProgram);
	//}
	{
		pShaderProgram->bind();
		/*pShaderProgram->setUniformValue("light_position_eye", glm::vec3(camera.getViewMatrix() * glm::vec4(fLightSourcePosition[0], fLightSourcePosition[1], fLightSourcePosition[2], 1.f)));
		pShaderProgram->setUniformValue("light_color", glm::vec3(fLightSourceColor[0], fLightSourceColor[1], fLightSourceColor[2]));*/
		pShaderProgram->setUniformValue("u_projectionMatrix", camera.getProjectionMatrix());
		pShaderProgram->setUniformValue("u_viewMatrix", camera.getViewMatrix());
		pShaderProgram->setUniformValue("u_shadowMap", GL_TEXTURE4 - GL_TEXTURE0);
		pShaderProgram->setUniformValue("u_lightDirection", glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

		glm::mat4 projectionLightMatrix = glm::mat4(1.0f);
		projectionLightMatrix = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);
		pShaderProgram->setUniformValue("u_projectionLightMatrix", projectionLightMatrix);


		glm::mat4 shadowLightMatrix = glm::mat4(1.0f);
		shadowLightMatrix = glm::rotate(shadowLightMatrix, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		shadowLightMatrix = glm::rotate(shadowLightMatrix, 40.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		pShaderProgram->setUniformValue("u_shadowLightMatrix", shadowLightMatrix);
		

		glm::mat4 lightMatrix = glm::mat4(1.0f);
		lightMatrix = glm::rotate(lightMatrix, -40.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		lightMatrix = glm::rotate(lightMatrix, -30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		pShaderProgram->setUniformValue("u_lightMatrix", lightMatrix);
		pShaderProgram->setUniformValue("u_lightPower", 10.0f);

		objTest->SetPosition(glm::vec3(5.f, 0.f, 0.f));
		objTest->SetRotation(glm::vec3(fAngles[0], fAngles[1], fAngles[2]));
		objTest->Draw(pShaderProgram);
	}

	UIModule::onWindowUpdateBegin();
	on_UIDraw();
	UIModule::onWindowUpdateDraw();

	//-------------------------------------------------//
	m_window->on_update();
	on_update();
}

int Application::start(const char* title, bool bAutoSize, unsigned int widnow_width, unsigned int widnow_height)
{

    m_window = std::make_unique<Window>(title, bAutoSize);
    camera.SetViewportSize(static_cast<float>(widnow_width), static_cast<float>(widnow_height));

	if (!InitShaders())
		return -1;
	InitCallbacks();
    Multimedia::InitSoundContext();

    objTest = ResourceManager::loadObject("test", "res/objects/plane.obj");	
	//m_pLightObj = std::dynamic_pointer_cast<LightBase>(ResourceManager::loadObject("light", "res/objects/cube.obj", ResourceManager::EObjectType::kLight));
    Renderer_OpenGL::EnableDepthTesting();

    while (!m_bCloseWindow)
    {
        draw();
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
			draw();
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

	pShaderProgram = ResourceManager::getShaderProgram("main_shader");
	if (!pShaderProgram->isCompiled())
	{
		LOG_INFO("The shader main_shader is not compiled");
		return false;
	}
	else
		LOG_INFO("The shader main_shader is compiled");

	/*pLightSourceShaderProgram = ResourceManager::getShaderProgram("light_shader");
	if (!pLightSourceShaderProgram->isCompiled())
	{
		LOG_INFO("The shader light_shader is not compiled");
		return false;
	}
	else
		LOG_INFO("The shader light_shader is compiled");*/

	return true;
}

