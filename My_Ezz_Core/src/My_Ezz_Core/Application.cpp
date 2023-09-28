

#include "My_Ezz_Core/Application.hpp"

#include "My_Ezz_Logger/Log.hpp"
#include "My_Ezz_Core/Window.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/Texture2D.hpp"
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

GLfloat positions[] = {
	//    position             normal            UV                  index

	   // FRONT
	   -1.0f, -1.f, -1.f,    -1.f,  0.f,  0.f,     0.f, 0.f,              // 0
	   -1.0f,  1.f, -1.f,    -1.f,  0.f,  0.f,     1.f, 0.f,              // 1
	   -1.0f,  1.f,  1.f,    -1.f,  0.f,  0.f,     1.f, 1.f,              // 2
	   -1.0f, -1.f,  1.f,    -1.f,  0.f,  0.f,     0.f, 1.f,              // 3

	   // BACK                                  
		1.0f, -1.f, -1.f,     1.f,  0.f,  0.f,     1.f, 0.f,              // 4
		1.0f,  1.f, -1.f,     1.f,  0.f,  0.f,     0.f, 0.f,              // 5
		1.0f,  1.f,  1.f,     1.f,  0.f,  0.f,     0.f, 1.f,              // 6
		1.0f, -1.f,  1.f,     1.f,  0.f,  0.f,     1.f, 1.f,              // 7

		// RIGHT
		-1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     0.f, 0.f,              // 8
		 1.0f,  1.f, -1.f,     0.f,  1.f,  0.f,     1.f, 0.f,              // 9
		 1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     1.f, 1.f,              // 10
		-1.0f,  1.f,  1.f,     0.f,  1.f,  0.f,     0.f, 1.f,              // 11

		// LEFT
		-1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     1.f, 0.f,              // 12
		 1.0f, -1.f, -1.f,     0.f, -1.f,  0.f,     0.f, 0.f,              // 13
		 1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     0.f, 1.f,              // 14
		-1.0f, -1.f,  1.f,     0.f, -1.f,  0.f,     1.f, 1.f,              // 15

		// TOP
		-1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 0.f,              // 16
		-1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 0.f,              // 17
		 1.0f,  1.f,  1.f,     0.f,  0.f,  1.f,     1.f, 1.f,              // 18
		 1.0f, -1.f,  1.f,     0.f,  0.f,  1.f,     0.f, 1.f,              // 19

		 // BOTTOM
		 -1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 1.f,              // 20
		 -1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 1.f,              // 21
		  1.0f,  1.f, -1.f,    0.f,  0.f, -1.f,     1.f, 0.f,              // 22
		  1.0f, -1.f, -1.f,    0.f,  0.f, -1.f,     0.f, 0.f,              // 23
};

GLint indeces[] = {
	    0,   1,  2,  2,  3,  0, // front
		4,   5,  6,  6,  7,  4, // back
		8,   9, 10, 10, 11,  8, // right
		12, 13, 14, 14, 15, 12, // left
		16, 17, 18, 18, 19, 16, // top
		20, 21, 22, 22, 23, 20  // bottom
};


void GenerateCircle(unsigned char* data,
    const unsigned int width,
    const unsigned int height,
    const unsigned int center_x, 
    const unsigned int center_y, 
    const unsigned int radius,
    const unsigned char color_r,
    const unsigned char color_g, 
    const unsigned char color_b)
{
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            if ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y) < radius * radius)
            {
				data[3 * (x + width * y) + 0] = color_r;
				data[3 * (x + width * y) + 1] = color_g;
				data[3 * (x + width * y) + 2] = color_b;
            }
        }
    }
}

void GenerateSmileTexture(unsigned char* data,
    const unsigned int width,
    const unsigned int height)
{
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            data[3 * (x + width * y) + 0] = 200;
            data[3 * (x + width * y) + 1] = 191;
            data[3 * (x + width * y) + 2] = 231;
        }
    }

    //face
    GenerateCircle(data, width, height, width * 0.5, height * 0.5, width * 0.4, 255, 255, 0);

    //smile
    GenerateCircle(data, width, height, width * 0.5, height * 0.4, width * 0.2, 0, 0, 0);
    GenerateCircle(data, width, height, width * 0.5, height * 0.45, width * 0.2, 255, 255, 0);

    //eyes
    GenerateCircle(data, width, height, width * 0.35, height * 0.6, width * 0.1, 255, 0, 255);
    GenerateCircle(data, width, height, width * 0.65, height * 0.6, width * 0.1, 0, 0, 255);
}

void GenerateQuadsTexture(unsigned char* data,
    const unsigned int width,
    const unsigned int height)
{
	for (unsigned int x = 0; x < width; ++x)
	{
		for (unsigned int y = 0; y < height; ++y)
		{
            if (x < width / 2 && y < height / 2 || x >= width / 2 && y >= height / 2)
            {
				data[3 * (x + width * y) + 0] = 0;
                data[3 * (x + width * y) + 1] = 0;
                data[3 * (x + width * y) + 2] = 0;
            }
            else
            {
				data[3 * (x + width * y) + 0] = 255;
				data[3 * (x + width * y) + 1] = 255;
				data[3 * (x + width * y) + 2] = 255;
            }
		}
	}
}

std::shared_ptr<ShaderProgram> pShaderProgram;
std::shared_ptr<ShaderProgram> pLightSourceShaderProgram;
std::shared_ptr<VertexBuffer> pCubePositionsVBO;
std::shared_ptr<IndexBuffer> pCubeIndexBuffer;
std::shared_ptr<Texture2D> pTexture_Smile;
std::shared_ptr<Texture2D> pTexture_Quads;
std::shared_ptr<VertexArray> pVAO_1;
std::shared_ptr<Object> objPhiramide;
std::shared_ptr<Object> objSphere;
std::shared_ptr<Object> objCube;

float m_backgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.0f };


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

	////light source
	{
		pLightSourceShaderProgram->bind();
		m_pLightObj->SetPosition(glm::vec3(fLightSourcePosition[0], fLightSourcePosition[1], fLightSourcePosition[2]));
		m_pLightObj->GetMesh()->SetShaderMatrixs(camera.getProjectionMatrix(), camera.getViewMatrix());
		m_pLightObj->SetColor(glm::vec3(fLightSourceColor[0], fLightSourceColor[1], fLightSourceColor[2]));
		m_pLightObj->SetScale(0.2);
		m_pLightObj->Draw(pLightSourceShaderProgram);
	}
	{
		pShaderProgram->bind();
		pShaderProgram->setVec3("light_position_eye", glm::vec3(camera.getViewMatrix() * glm::vec4(fLightSourcePosition[0], fLightSourcePosition[1], fLightSourcePosition[2], 1.f)));
		pShaderProgram->setVec3("light_color", glm::vec3(fLightSourceColor[0], fLightSourceColor[1], fLightSourceColor[2]));
		pShaderProgram->setFloat("ambient_factor", fAmbientFactor);
		pShaderProgram->setFloat("diffuse_factor", fDiffuseFactor);
		pShaderProgram->setFloat("specular_factor", fSpecularFactor);
		pShaderProgram->setFloat("shininess", fShininess);

		objSphere->GetMesh()->SetShaderMatrixs(camera.getProjectionMatrix(), camera.getViewMatrix());
		objSphere->SetPosition(glm::vec3(-5.0, 0.f, 0.f));
		objSphere->SetRotation(glm::vec3(fAngles[0], fAngles[1], fAngles[2]));
		objSphere->Draw(pShaderProgram);

		objPhiramide->GetMesh()->SetShaderMatrixs(camera.getProjectionMatrix(), camera.getViewMatrix());
		objPhiramide->SetPosition(glm::vec3(5.0, 0.f, 0.f));
		objPhiramide->SetRotation(glm::vec3(fAngles[0], fAngles[1], fAngles[2]));
		objPhiramide->Draw(pShaderProgram);

		objCube->GetMesh()->SetShaderMatrixs(camera.getProjectionMatrix(), camera.getViewMatrix());
		objCube->SetPosition(glm::vec3(5.0, 5.f, 0.f));
		objCube->SetRotation(glm::vec3(fAngles[0], fAngles[1], fAngles[2]));
		objCube->Draw(pShaderProgram);
	}

	UIModule::onWindowUpdateBegin();
	on_UIDraw();
	UIModule::onWindowUpdateDraw();

	//-------------------------------------------------//
	m_window->on_update();
	on_update();
}

int Application::start(unsigned int widnow_width, unsigned int widnow_height, const char* title)
{
    m_window = std::make_unique<Window>(title, widnow_width, widnow_height);
    camera.SetViewportSize(static_cast<float>(widnow_width), static_cast<float>(widnow_height));

    Multimedia::InitSoundContext();

	ResourceManager::loadShaders("main_shader", "res/shaders/main_shader.vert", "res/shaders/main_shader.frag");
	ResourceManager::loadShaders("light_shader", "res/shaders/light_shader.vert", "res/shaders/light_shader.frag");

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


	const unsigned int width = 1000;
	const unsigned int height = 1000;
	const unsigned int channels = 3;

	auto* data = new unsigned char[width * height * channels];

	GenerateSmileTexture(data, width, height);
	pTexture_Smile = std::make_unique<Texture2D>(data, width, height);
	pTexture_Smile->bind(0);

	delete[] data;


    //-----------------------------------------------------//
    pShaderProgram = ResourceManager::getShaderProgram("main_shader");
    if (!pShaderProgram->isCompiled())
    {
        return false;
    }



    objPhiramide = ResourceManager::loadObject("phiramide", "res/objects/phiramid.obj");
    objSphere = ResourceManager::loadObject("sphere", "res/objects/sphere.obj");
    objCube = ResourceManager::loadObject("cube", "res/objects/cube.obj");

    
	pLightSourceShaderProgram = ResourceManager::getShaderProgram("light_shader");
	if (!pLightSourceShaderProgram->isCompiled())
	{
		return false;
	}
	m_pLightObj = std::dynamic_pointer_cast<LightBase>(ResourceManager::loadObject("light", "res/objects/cube.obj", ResourceManager::EObjectType::kLight));
    Renderer_OpenGL::EnableDepthTesting();
	/* std::thread tr([&]() {timer(); });
	 tr.detach();*/
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

