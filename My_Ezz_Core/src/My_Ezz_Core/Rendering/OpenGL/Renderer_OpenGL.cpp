#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "My_Ezz_Core/Log.hpp"

using namespace My_Ezz;

bool Renderer_OpenGL::init(GLFWwindow* _pWindow)
{
	glfwMakeContextCurrent(_pWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        LOG_CRITICAL("Failed to initialize GLAD");
        return false;
    }
    LOG_INFO("OpenGL context has been initialized successfully!");
    LOG_INFO("Vendor: {0}", getVendorStr());
    LOG_INFO("Renderer: {0}", getRendererStr());
    LOG_INFO("Version: {0}", getVersionStr());

    return true;
}

void Renderer_OpenGL::draw(const VertexArray& _vertexArray)
{
    _vertexArray.bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_vertexArray.getIndecesCount()), GL_UNSIGNED_INT, nullptr);
}

void Renderer_OpenGL::setClearColor(const float _red, const float _green, const float _blue, const float _alpha)
{
    glClearColor(_red, _green, _blue, _alpha);
}

void Renderer_OpenGL::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer_OpenGL::setViewport(const unsigned int _width, const unsigned int _height, const unsigned int _leftOffset, const unsigned int _bottomOffset)
{
    glViewport(_leftOffset, _bottomOffset, _width, _height);
}


const char* Renderer_OpenGL::getVendorStr()
{
    return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

const char* Renderer_OpenGL::getRendererStr()
{
    return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

const char* Renderer_OpenGL::getVersionStr()
{
    return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

void Renderer_OpenGL::EnableDepthTesting()
{
    glEnable(GL_DEPTH_TEST);
}

void Renderer_OpenGL::DisableDepthTesting()
{
    glDisable(GL_DEPTH_TEST);
}