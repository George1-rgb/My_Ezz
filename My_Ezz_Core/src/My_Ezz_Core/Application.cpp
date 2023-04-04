

#include "My_Ezz_Core/Application.hpp"

#include "My_Ezz_Core/Log.hpp"
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

#include <GLFW/glfw3.h>
#include <iostream>

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
    GenerateCircle(data, width, height, width * 0.35, height * 0.6, width * 0.07, 255, 0, 255);
    GenerateCircle(data, width, height, width * 0.65, height * 0.6, width * 0.07, 0, 0, 255);
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

const char* vertex_shader =
R"(#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;


uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform int current_frame;

out vec2 tex_coord_Smile;
out vec2 tex_coord_Quads;


void main() {
    tex_coord_Smile = texture_coord;
    tex_coord_Quads = texture_coord + vec2(current_frame/1000.0f, current_frame/1000.0f);
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertex_position, 1.0);
}
)";

const char* fragment_shader =
R"(#version 460
in vec2 tex_coord_Smile;
in vec2 tex_coord_Quads;

layout (binding = 0) uniform sampler2D InTexture_Smile;
layout (binding = 1) uniform sampler2D InTexture_Quads;

out vec4 frag_color;

void main() {
frag_color = texture(InTexture_Smile, tex_coord_Smile);
//frag_color = texture(InTexture_Smile, tex_coord_Smile) * texture(InTexture_Quads, tex_coord_Quads);
}
)";
std::unique_ptr<ShaderProgram> pShaderProgram;
std::unique_ptr<VertexBuffer> pCubePositionsVBO;
std::unique_ptr<IndexBuffer> pCubeIndexBuffer;
std::unique_ptr<Texture2D> pTexture_Smile;
std::unique_ptr<Texture2D> pTexture_Quads;
std::unique_ptr<VertexArray> pVAO_1;

float scale[3] = { 1.0f, 1.0f, 1.0f };
float rotate = 0.0f;
float translate[3] = { 0.0f, 0.0f, 1.0f };

float m_backgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.0f };


std::array<glm::vec3, 5> positionsCubes =
{
    glm::vec3(-2.f, -2.f, -4.f),
    glm::vec3(-5.f, 0.f, 3.f),
    glm::vec3(2.f, 1.f, 2.f),
    glm::vec3(4.f, -3.f, 3.f),
    glm::vec3(1.f, -7.f, 1.f),
};

Application::Application()
{
    LOG_INFO("Starting Application");
    
}

Application::~Application()
{
    LOG_INFO("Closing Application");
}

void Application::draw()
{
	//----------------------------------------//
	Renderer_OpenGL::setClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
	Renderer_OpenGL::clear();



	pShaderProgram->bind();

	glm::mat4 scaleMatrix(scale[0], 0, 0, 0,
		0, scale[1], 0, 0,
		0, 0, scale[2], 0,
		0, 0, 0, 1);

	float rotateInRadians = glm::radians(rotate);
	glm::mat4 rotateMatrix(
		cos(rotateInRadians), sin(rotateInRadians), 0, 0,
		-sin(rotateInRadians), cos(rotateInRadians), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);


	glm::mat4 translateMatrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		translate[0], translate[1], translate[2], 1
	);

	glm::mat4 modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;

	pShaderProgram->setMatrix4("modelMatrix", modelMatrix);
	//pShaderProgram->setInt("current_frame", frame++);


	glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
	pShaderProgram->setMatrix4("viewProjectionMatrix", viewProjectionMatrix);

	Renderer_OpenGL::draw(*pVAO_1);

	for (const glm::vec3 curPos : positionsCubes)
	{
		glm::mat4 translateMatrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			curPos[0], curPos[1], curPos[2], 1
		);
		pShaderProgram->setMatrix4("modelMatrix", translateMatrix);
		Renderer_OpenGL::draw(*pVAO_1);
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

    m_eventDispatcher.addEventListener<EventMouseMoved>(
        [](EventMouseMoved& event) 
        {
            //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
        });

    m_eventDispatcher.addEventListener<EventWindowResize>(
        [&](EventWindowResize& event)
        {
            LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
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

    GenerateQuadsTexture(data, width, height);
    pTexture_Quads = std::make_unique<Texture2D>(data, width, height);
    pTexture_Quads->bind(1);

    delete[] data;


    //-----------------------------------------------------//
    pShaderProgram = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
    if (!pShaderProgram->isCompiled())
    {
        return false;
    }


    BufferLayout bufferLayout_2vec3_vec2
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3,
        ShaderDataType::Float2
    };
    pVAO_1 = std::make_unique<VertexArray>();

    pCubePositionsVBO = std::make_unique<VertexBuffer>(positions, sizeof(positions), bufferLayout_2vec3_vec2);
    pCubeIndexBuffer = std::make_unique<IndexBuffer>(indeces, sizeof(indeces) / sizeof(GLuint));
    pVAO_1->addVertexBuffer(*pCubePositionsVBO);
    pVAO_1->setIndexBuffer(*pCubeIndexBuffer);
    //--------------------------------------------------------------//

    static int frame = 0;
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
