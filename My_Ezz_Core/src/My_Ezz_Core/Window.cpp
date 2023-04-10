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

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
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

    glfwSetKeyCallback(m_window, 
        [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
	        switch (action)
	        {
            case GLFW_PRESS:
            {
                EventKeyPressed event(static_cast<KeyCode>(key), false);
                data.eventCallbackFn(event);
                break;
            }
			case GLFW_RELEASE:
            {
                EventKeyReleased event(static_cast<KeyCode>(key));
                data.eventCallbackFn(event);
                break;
            }
			case GLFW_REPEAT:
            {
                EventKeyPressed event(static_cast<KeyCode>(key), true);
                data.eventCallbackFn(event);
                break;
            }
	        }

        });

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

	glfwSetMouseButtonCallback(m_window,
		[](GLFWwindow* window, int key, int action, int mods)
		{
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
           
			double x_pos;
			double y_pos;
			glfwGetCursorPos(window, &x_pos, &y_pos);

			switch (action)
			{
			case GLFW_PRESS:
			{
				EventMouseButtonPressed event(static_cast<MouseButton>(key), x_pos, y_pos);
				data.eventCallbackFn(event);
				break;
			}
			case GLFW_RELEASE:
			{
				EventMouseButtonReleased event(static_cast<MouseButton>(key), x_pos, y_pos);
				data.eventCallbackFn(event);
				break;
			}
			
			}

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

glm::vec2 Window::GetCurrentCursorPosition() const
{
	double x_pos;
	double y_pos;
	glfwGetCursorPos(m_window, &x_pos, &y_pos);
    return { x_pos, y_pos };
}
