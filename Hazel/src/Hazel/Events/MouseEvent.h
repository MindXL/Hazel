#pragma once

#include "Event.h"

namespace Hazel
{
	class MouseMovedEvent final : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			: m_MouseX{x}, m_MouseY{y} {}

		[[nodiscard]] float GetX() const { return m_MouseX; }
		[[nodiscard]] float GetY() const { return m_MouseY; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: (" << m_MouseX << ", " << m_MouseY << ')';
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		float m_MouseX, m_MouseY;
	};

	class MouseButtonEvent : public Event
	{
	public:
		[[nodiscard]] int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryInput)

	protected:
		explicit MouseButtonEvent(const int button)
			: m_Button{button} {}

		int m_Button;
	};

	class MouseButtonPressedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonPressedEvent(const int button)
			: MouseButtonEvent{button} {}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class MouseButtonReleasedEvent final : public MouseButtonEvent
	{
	public:
		explicit MouseButtonReleasedEvent(const int button)
			: MouseButtonEvent{button} {}

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};

	class MouseScrolledEvent final : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_XOffset{xOffset}, m_YOffset{yOffset} {}

		[[nodiscard]] float GetXOffset() const { return m_XOffset; }
		[[nodiscard]] float GetYOffset() const { return m_YOffset; }

		[[nodiscard]] std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: (" << m_XOffset << ", " << m_YOffset << ')';
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
		EVENT_CLASS_TYPE(MouseScrolled)

	private:
		float m_XOffset, m_YOffset;
	};
}
