#pragma once

#include "pch.h"

#include "Hazel/Core/Core.h"

namespace Hazel
{
	// Events run by blocking the process for now.

	enum class EventType
	{
		None = 0,
		WindowResize, WindowClose, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseMoved, MouseButtonPressed, MouseButtonReleased, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) [[nodiscard]] static EventType GetStaticType() { return EventType::##type; }\
							   [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }\
							   [[nodiscard]] const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) [[nodiscard]] int GetCategoryFlags() const override { return category; }

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual ~Event() = default;

		bool Handled = false;

		[[nodiscard]] virtual EventType GetEventType() const = 0;
		[[nodiscard]] virtual const char* GetName() const = 0;
		[[nodiscard]] virtual int GetCategoryFlags() const = 0;
		[[nodiscard]] virtual std::string ToString() const { return GetName(); }

		[[nodiscard]] bool IsInCategory(const EventCategory category) const { return GetCategoryFlags() & category; }
	};

	class EventDispatcher
	{
	public:
		explicit EventDispatcher(Event& event)
			: m_Event{event} {}

		template<typename T, typename Func>
		bool Dispatch(const Func& func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		return os << event.ToString();
	}
}
