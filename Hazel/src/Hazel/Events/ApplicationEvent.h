#pragma once

#include "Event.h"

namespace Hazel
{
	class WindowResizeEvent final : public Event
	{
	public:
		WindowResizeEvent(const unsigned int width, const unsigned int height)
			: m_Width{width}, m_Height{height} {}

		[[nodiscard]] unsigned int GetWidth() const { return m_Width; }
		[[nodiscard]] unsigned int GetHeight() const { return m_Height; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: (" << m_Width << ", " << m_Height << ')';
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowResize)

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent final : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(WindowClose)
	};

	class AppTickEvent final : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppTick)
	};

	class AppUpdateEvent final : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppUpdate)
	};

	class AppRenderEvent final : public Event
	{
	public:
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
		EVENT_CLASS_TYPE(AppRender)
	};
}
