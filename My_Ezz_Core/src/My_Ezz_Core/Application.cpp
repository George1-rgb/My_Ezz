

#include "My_Ezz_Core/Application.hpp"

#include "My_Ezz_Core/Log.hpp"
#include "My_Ezz_Core/Window.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Camera.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Modules/UIModule.hpp"

#include <imgui/imgui.h>
#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>

#include <GLFW/glfw3.h>
#include <iostream>

using namespace My_Ezz;

GLfloat positions_colors[] = {
    -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f
};

GLint indeces[] = {
    0, 1, 2, 3, 2, 1
};

const char* vertex_shader =
R"(#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
out vec3 color;
void main() {
    color = vertex_color;
    gl_Position = viewProjectionMatrix * modelMatrix * vec4(vertex_position, 1.0);
}
)";

const char* fragment_shader =
"#version 460\n"
"in vec3 color;"
"out vec4 frag_color;"
"void main() {"
"frag_color = vec4(color, 1.0);"
"}";
std::unique_ptr<ShaderProgram> pShaderProgram;
std::unique_ptr<VertexBuffer> pPositoinsColorsVBO;
std::unique_ptr<IndexBuffer> pindexBuffer;;
std::unique_ptr<VertexArray> pVAO_1;

float scale[3] = { 1.0f, 1.0f, 1.0f };
float rotate = 0.0f;
float translate[3] = { 0.0f, 0.0f, 1.0f };

float m_backgroundColor[4] = { 0.33f, 0.33f, 0.33f, 0.0f };

Application::Application()
{
    LOG_INFO("Starting Application");
    
}

Application::~Application()
{
    LOG_INFO("Closing Application");
}

int Application::start(unsigned int widnow_width, unsigned int widnow_height, const char* title)
{
   
    m_window = std::make_unique<Window>(title, widnow_width, widnow_height);
    m_eventDispatcher.addEventListener<EventMouseMoved>(
        [](EventMouseMoved& event) 
        {
            //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
        });

    m_eventDispatcher.addEventListener<EventWindowResize>(
        [](EventWindowResize& event)
        {
            LOG_INFO("[Resized] Changed size to {0}x{1}", event.width, event.height);
        });

    m_eventDispatcher.addEventListener<EventWindowClose>(
        [&](EventWindowClose& event)
        {
            LOG_INFO("[WindowClose]");
            m_bCloseWindow = true;
        });

    m_window->set_event_callback(
        [&](BaseEvent& event)
        {
            m_eventDispatcher.dispatch(event);
        });

    //-----------------------------------------------------//
    pShaderProgram = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
    if (!pShaderProgram->isCompiled())
    {
        return false;
    }


    BufferLayout bufferLayout_2vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
    pVAO_1 = std::make_unique<VertexArray>();

    pPositoinsColorsVBO = std::make_unique<VertexBuffer>(positions_colors, sizeof(positions_colors), bufferLayout_2vec3);
    pindexBuffer = std::make_unique<IndexBuffer>(indeces, sizeof(indeces) / sizeof(GLuint));
    pVAO_1->addVertexBuffer(*pPositoinsColorsVBO);
    pVAO_1->setIndexBuffer(*pindexBuffer);
    //--------------------------------------------------------------//

    while (!m_bCloseWindow)
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
        camera.setPositionAndRotation(glm::vec3(cameraPosition[0], cameraPosition[1], cameraPosition[2]),
            glm::vec3(cameraRotation[0], cameraRotation[1], cameraRotation[2]));
        camera.setProjectionMode(perspectiveCamera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
        glm::mat4 viewProjectionMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
        pShaderProgram->setMatrix4("viewProjectionMatrix", viewProjectionMatrix);

        Renderer_OpenGL::draw(*pVAO_1);

        //----------//
        UIModule::onWindowUpdateBegin();
        bool show = true;
        UIModule::ShowExampleAppDockSpace(&show);

        ImGui::ShowDemoWindow();

        ImGui::Begin("Background color window");
        ImGui::ColorEdit4("Background color", m_backgroundColor);
        ImGui::SliderFloat3("scale", scale, 0.0f, 2.0f);
        ImGui::SliderFloat("rotate", &rotate, 0.0f, 360.0f);
        ImGui::SliderFloat3("translate", translate, -1.0f, 1.0f);

        ImGui::SliderFloat3("Camera position", cameraPosition, -10.0f, 10.0f);
        ImGui::SliderFloat3("Camera rotation", cameraRotation, 0.0f, 360.0f);
        ImGui::Checkbox("Perspective camera", &perspectiveCamera);
        ImGui::End();
        //----------//
        on_UIDraw();

        UIModule::onWindowUpdateDraw();
        
        //-------------------------------------------------//
        m_window->on_update();
        on_update();
    }
    m_window = nullptr;

    return 0;
}
