#pragma once

#include "pch.h"

#include "Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		explicit WindowProps(std::string title = "Hazel Engine",
		                     const unsigned int width = 1280,
		                     const unsigned int height = 720)
			: Title{std::move(title)}, Width{width}, Height{height} {}
	};

	// Interface representing a desktop system based Window.
	class HAZEL_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		[[nodiscard]] virtual void* GetNativeWindow() const = 0;
		static Scope<Window> Create(const WindowProps& props = WindowProps());

		[[nodiscard]] virtual unsigned int GetWidth() const = 0;
		[[nodiscard]] virtual unsigned int GetHeight() const = 0;

		virtual void OnUpdate() = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		[[nodiscard]] virtual bool IsVSync() const = 0;
	};
}
