#pragma once
#include "My_Ezz_Core/Event.hpp"
#include <string>
#include <functional>

struct GLFWwindow;

namespace My_Ezz
{
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(BaseEvent&)>;

		Window(std::string title, const unsigned int& width, const unsigned int& height);
		~Window();
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		Window& operator=(const Window&) = delete;
		Window& operator=(Window&&) = delete;

		
		void on_update();
		unsigned int get_width() const { return m_data.width; }
		unsigned int get_height() const { return m_data.height; }
		void set_event_callback(const EventCallbackFn& callback)
		{
			m_data.eventCallbackFn = callback;
		}

	private:

		struct WindowData
		{
			WindowData(std::string _title, const unsigned int& _width, const unsigned int& _height)
				: title(std::move(_title)), width(_width), height(_height) {}
			unsigned int width;
			unsigned int height;
			std::string title;
			EventCallbackFn eventCallbackFn;
		};

		int init();
		void shutDown();

		GLFWwindow* m_window = nullptr;
		WindowData m_data;

	};
}