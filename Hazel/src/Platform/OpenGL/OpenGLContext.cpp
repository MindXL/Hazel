#include "pch.h"

#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Hazel
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle{windowHandle}
	{
		HZ_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		HZ_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);

#ifdef HZ_ENABLE_ASSERTS
		const int version = gladLoadGL(glfwGetProcAddress);
		HZ_CORE_ASSERT(version, "Failed to initialize Glad(OpenGL context)!");
#else
		gladLoadGL(glfwGetProcAddress);
#endif // HZ_ENABLE_ASSERTS

		HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

#ifdef HZ_ENABLE_ASSERTS
		int versionMajor, versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		HZ_CORE_ASSERT((versionMajor == 4 && versionMinor >= 6)|| versionMajor > 4,
		               "Hazel requires at least OpenGL version 4.6!");
#endif // HZ_ENABLE_ASSERTS
	}

	void OpenGLContext::SwapBuffers()
	{
		HZ_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}
