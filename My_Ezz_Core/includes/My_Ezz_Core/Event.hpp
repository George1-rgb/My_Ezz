#pragma once
#include <functional>
#include <array>
#include "Keys.hpp"

namespace My_Ezz
{
	enum class EventType
	{
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,


		EventsCount
	};

	struct BaseEvent
	{
		virtual ~BaseEvent() = default;
		virtual EventType getType() const = 0;
	};

	class EventDispatcher
	{
	public:
		template<typename EventType>
		void addEventListener(std::function<void(EventType&)> callback)
		{
			auto baseCallback = [func = std::move(callback)](BaseEvent& e)
			{
				if (e.getType() == EventType::type)
				{
					func(static_cast<EventType&>(e));
				}
			};
			m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
		}

		void dispatch(BaseEvent& event)
		{
			auto& callBack = m_eventCallbacks[static_cast<size_t>(event.getType())];
			if (callBack)
			{
				callBack(event);
			}
		}
	private:
		std::array< std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;
	};

	struct EventMouseMoved : public BaseEvent
	{
		EventMouseMoved(const double new_x, const double new_y)
			: x(new_x), y(new_y)
		{
		}
		virtual EventType getType() const override
		{
			return type;
		}
		double x;
		double y;
		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent
	{
		EventWindowResize(const unsigned int new_width, const unsigned int new_height)
			: width(new_width), height(new_height)
		{
		}

		virtual EventType getType() const override
		{
			return type;
		}

		unsigned int width;
		unsigned int height;
		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent
	{
		virtual EventType getType() const override
		{
			return type;
		}
		static const EventType type = EventType::WindowClose;
	};

	struct EventKeyPressed : public BaseEvent
	{
		EventKeyPressed(const KeyCode key_Code, const bool bRepeated)
			: key_Code(key_Code), bRepeated(bRepeated)
		{
		}

		virtual EventType getType() const override
		{
			return type;
		}
		KeyCode key_Code;
		bool bRepeated;
		static const EventType type = EventType::KeyPressed;
	};

	struct EventKeyReleased : public BaseEvent
	{
		EventKeyReleased(const KeyCode key_Code)
			: key_Code(key_Code)
		{
		}

		virtual EventType getType() const override
		{
			return type;
		}
		KeyCode key_Code;

		static const EventType type = EventType::KeyReleased;
	};


	struct EventMouseButtonPressed : public BaseEvent
	{
		EventMouseButtonPressed(const MouseButton key_Code, const bool bRepeated)
			: key_Code(key_Code), bRepeated(bRepeated)
		{
		}

		virtual EventType getType() const override
		{
			return type;
		}
		MouseButton key_Code;
		bool bRepeated;
		static const EventType type = EventType::MouseButtonPressed;
	};

	struct EventMouseButtonReleased : public BaseEvent
	{
		EventMouseButtonReleased(const MouseButton key_Code)
			: key_Code(key_Code)
		{
		}

		virtual EventType getType() const override
		{
			return type;
		}
		MouseButton key_Code;

		static const EventType type = EventType::MouseButtonReleased;
	};
}