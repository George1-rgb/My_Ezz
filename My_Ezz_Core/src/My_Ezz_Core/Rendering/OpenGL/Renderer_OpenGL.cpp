#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.hpp"
#include "My_Ezz_Core/Log.hpp"

using namespace My_Ezz;

const char* glSourceToString(const GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API: return "DEBUG_SOURCE_API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "DEBUG_SOURCE_WINDOW_SYSTEM";
        case GL_DEBUG_SOURCE_SHADER_COMPILER: return "DEBUG_SOURCE_SHADER_COMPILER";
        case GL_DEBUG_SOURCE_THIRD_PARTY: return "DEBUG_SOURCE_THIRD_PARTY";
        case GL_DEBUG_SOURCE_APPLICATION: return "DEBUG_SOURCE_APPLICATION";
        case GL_DEBUG_SOURCE_OTHER: return "DEBUG_SOURCE_OTHER";

        default: return "UNKNOWN_DEBUG_SOURCE";
    }
}

const char* glTypeToString(const GLenum type)
{
    switch (type)
    {
	    case GL_DEBUG_TYPE_ERROR: return "DEBUG_TYPE_ERROR";
	    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEBUG_TYPE_DEPRECATED_BEHAVIOR";
	    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "DEBUG_TYPE_UNDEFINED_BEHAVIOR";
	    case GL_DEBUG_TYPE_PORTABILITY: return "DEBUG_TYPE_PORTABILITY";
	    case GL_DEBUG_TYPE_PERFORMANCE: return "DEBUG_TYPE_PERFORMANCE";
	    case GL_DEBUG_TYPE_MARKER: return "DEBUG_TYPE_MARKER";
	    case GL_DEBUG_TYPE_PUSH_GROUP: return "DEBUG_TYPE_PUSH_GROUP";
	    case GL_DEBUG_TYPE_POP_GROUP: return "DEBUG_TYPE_POP_GROUP";
	    case GL_DEBUG_TYPE_OTHER: return "DEBUG_TYPE_OTHER";

	    default: return "UNKNOWN_DEBUG_TYPE";
    }
}

bool Renderer_OpenGL::init(GLFWwindow* _pWindow)
{
	glfwMakeContextCurrent(_pWindow);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        LOG_CRITICAL("Failed to initialize GLAD");
        return false;
    }
    LOG_INFO("OpenGL context has been initialized successfully!");
    LOG_INFO("OpenGL Vendor: {0}", getVendorStr());
    LOG_INFO("OpenGL Renderer: {0}", getRendererStr());
    LOG_INFO("OpenGL Version: {0}", getVersionStr());

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageCallback([](GLenum source, 
                              GLenum type,
                              GLuint id,
                              GLenum severity,
                              GLsizei length, 
                              const GLchar* message, 
                              const void* userParam)
        {
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:
				LOG_ERROR("OpenGL Error: [{0}:{1}]({2}): {3}",
					glSourceToString(source),
					glTypeToString(type),
					id,
					message);
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				LOG_WARNING("OpenGL Warning: [{0}:{1}]({2}): {3}",
					glSourceToString(source),
					glTypeToString(type),
					id,
					message);
				break;
			case GL_DEBUG_SEVERITY_LOW:
				LOG_INFO("OpenGL Info: [{0}:{1}]({2}): {3}",
					glSourceToString(source),
					glTypeToString(type),
					id,
					message);
				break;
			case GL_DEBUG_SEVERITY_NOTIFICATION:
				LOG_INFO("OpenGL Notificaton: [{0}:{1}]({2}): {3}",
					glSourceToString(source),
					glTypeToString(type),
					id,
					message);
				break;
			default:
				LOG_ERROR("OpenGL Error: [{0}:{1}] ({2}) : {3}",
					glSourceToString(source),
					glTypeToString(type),
					id,
					message);
			}
        }, nullptr);

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