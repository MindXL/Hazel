#include "pch.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle{ windowHandle }
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);

		int version = gladLoadGL(glfwGetProcAddress);
		HZ_CORE_ASSERT(version, "Failed to initialize Glad(OpenGL context)!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
