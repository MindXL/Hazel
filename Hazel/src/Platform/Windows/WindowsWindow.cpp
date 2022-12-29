#include "pch.h"

#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

namespace Hazel {
	static bool s_GLFWInitialized = false;

	static void GFLWErrorCallback(int error_code, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{// Only initialze GLFW for once.
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GFLWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		// TODO: Fix or close VS's horrible auto formatting for lambda functions when saved as shown below.
		glfwSetWindowSizeCallback(m_Window,
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
		data.Width = width;
		data.Height = height;

		WindowResizeEvent event{ (unsigned int)width,(unsigned int)height };
		data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window,
			[](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		WindowCloseEvent event;
		data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			KeyPressedEvent event{ key,0 };
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			KeyReleasedEvent event{ key };
			data.EventCallback(event);
			break;
		}
		case GLFW_REPEAT:
		{
			KeyPressedEvent event{ key,1 };
			data.EventCallback(event);
			break;
		}
		}
			});

		glfwSetCursorPosCallback(m_Window,
			[](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseMovedEvent event{ (float)xpos,(float)ypos };
		data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window,
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		switch (action)
		{
		case GLFW_PRESS:
		{
			MouseButtonPressedEvent event{ button };
			data.EventCallback(event);
			break;
		}
		case GLFW_RELEASE:
		{
			MouseButtonReleasedEvent event{ button };
			data.EventCallback(event);
			break;
		}
		}
			});

		glfwSetScrollCallback(m_Window,
			[](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

		MouseScrolledEvent event{ (float)xoffset,(float)yoffset };
		data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}
