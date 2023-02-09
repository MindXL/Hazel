#pragma once

#include "Hazel/Core/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Hazel
{
	class WindowsWindow final : public Window
	{
	public:
		explicit WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		[[nodiscard]] void* GetNativeWindow() const override { return m_Window; }
		[[nodiscard]] unsigned int GetWidth() const override { return m_Data.Width; }
		[[nodiscard]] unsigned int GetHeight() const override { return m_Data.Height; }

		void OnUpdate() override;

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		[[nodiscard]] bool IsVSync() const override { return m_Data.VSync; }

	private:
		void Init(const WindowProps& props);
		void Shutdown() const { glfwDestroyWindow(m_Window); }

	private:
		GLFWwindow* m_Window = nullptr;
		Scope<GraphicsContext> m_Context = nullptr;

		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0, Height = 0;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}
