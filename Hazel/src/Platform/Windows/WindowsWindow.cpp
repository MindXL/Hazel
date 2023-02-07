#include "pch.h"

#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Hazel
{
	static bool s_GLFWInitialized = false;

	static void GFLWErrorCallback(int error_code, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
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
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(const bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// Only initialze GLFW for once.
#ifdef HZ_ENABLE_ASSERTS
			const int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not initialize GLFW!");
#else
			glfwInit();
#endif // HZ_ENABLE_ASSERTS

			glfwSetErrorCallback(GFLWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, props.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window,
		                          [](GLFWwindow* window, int width, int height)-> void
		                          {
			                          WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			                          data.Width = width;
			                          data.Height = height;

			                          WindowResizeEvent event{(unsigned int)width, (unsigned int)height};
			                          data.EventCallback(event);
		                          });

		glfwSetWindowCloseCallback(m_Window,
		                           [](GLFWwindow* window)-> void
		                           {
			                           const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			                           WindowCloseEvent event;
			                           data.EventCallback(event);
		                           });

		glfwSetKeyCallback(m_Window,
		                   [](GLFWwindow* window, int key, int scancode, int action, int mods)-> void
		                   {
			                   const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			                   switch (action)
			                   {
			                   case GLFW_PRESS:
				                   {
					                   KeyPressedEvent event{key, 0};
					                   data.EventCallback(event);
					                   break;
				                   }
			                   case GLFW_RELEASE:
				                   {
					                   KeyReleasedEvent event{key};
					                   data.EventCallback(event);
					                   break;
				                   }
			                   case GLFW_REPEAT:
				                   {
					                   KeyPressedEvent event{key, 1};
					                   data.EventCallback(event);
					                   break;
				                   }
			                   default: HZ_CORE_ASSERT(false, "Unknown key event!");
			                   }
		                   });

		glfwSetCharCallback(m_Window,
		                    // codepoint: The Unicode code point of the character.
		                    [](GLFWwindow* window, unsigned int codepoint)-> void
		                    {
			                    const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			                    KeyTypedEvent event{(int)codepoint};
			                    data.EventCallback(event);
		                    });

		glfwSetCursorPosCallback(m_Window,
		                         [](GLFWwindow* window, double xpos, double ypos)-> void
		                         {
			                         const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			                         MouseMovedEvent event{(float)xpos, (float)ypos};
			                         data.EventCallback(event);
		                         });

		glfwSetMouseButtonCallback(m_Window,
		                           [](GLFWwindow* window, int button, int action, int mods)-> void
		                           {
			                           const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			                           switch (action)
			                           {
			                           case GLFW_PRESS:
				                           {
					                           MouseButtonPressedEvent event{button};
					                           data.EventCallback(event);
					                           break;
				                           }
			                           case GLFW_RELEASE:
				                           {
					                           MouseButtonReleasedEvent event{button};
					                           data.EventCallback(event);
					                           break;
				                           }
			                           default: HZ_CORE_ASSERT(false, "Unknown key event!");
			                           }
		                           });

		glfwSetScrollCallback(m_Window,
		                      [](GLFWwindow* window, double xoffset, double yoffset)-> void
		                      {
			                      const WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			                      MouseScrolledEvent event{(float)xoffset, (float)yoffset};
			                      data.EventCallback(event);
		                      });
	}
}
