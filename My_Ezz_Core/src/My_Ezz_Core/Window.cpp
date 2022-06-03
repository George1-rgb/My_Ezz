#include "My_Ezz_Core/Window.hpp"
#include "My_Ezz_Core/Log.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Modules/UIModule.hpp"

#include <GLFW/glfw3.h>




using namespace My_Ezz;

Window::Window(std::string title, const unsigned int& width, const unsigned int& height)
    : m_data({ std::move(title), width, height })
{
	int resultCode = init();

    
}

Window::~Window()
{
	shutDown();
}

void Window::on_update()
{
    

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

int Window::init()
{

    LOG_INFO("Creating window '{0}' width size {1}x{2}", m_data.title, m_data.width, m_data.height);
    glfwSetErrorCallback([](int _errorCode, const char* _description)
        {
            LOG_CRITICAL("[GLFW ERROR]: {0}", _description);
        }
    );
    if (!glfwInit())
    {
       LOG_CRITICAL("Can't initialize GLFW!");
       return -1;
    }

    
    m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
    if (!m_window)
    {
        LOG_CRITICAL("Can't create window {0}!", m_data.title);
        return -2;
    }

    if (!Renderer_OpenGL::init(m_window))
    {
        LOG_CRITICAL("Falied to OpenGL renderer!");
        return -3;
    }

    glfwSetWindowUserPointer(m_window, &m_data);
    glfwSetWindowSizeCallback(m_window, 
        [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = width;
            data.height = height;
            EventWindowResize event(width, height);
            data.eventCallbackFn(event);
        });

    glfwSetCursorPosCallback(m_window, 
        [](GLFWwindow* window, double x, double y)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            EventMouseMoved event(x, y);
            data.eventCallbackFn(event);
        });

    glfwSetWindowCloseCallback(m_window,
        [](GLFWwindow* window)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            EventWindowClose event;
            data.eventCallbackFn(event);
        });

    glfwSetFramebufferSizeCallback(m_window,
        [](GLFWwindow* window, int width, int height)
        {
            Renderer_OpenGL::setViewport(width, height);
        });

    UIModule::onWindowCreate(m_window);

   

    return 0;

}

void Window::shutDown()
{
    UIModule::onWindowClose();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}
