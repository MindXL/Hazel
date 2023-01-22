#include "pch.h"

#include "WindowsInput.h"
#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hazel
{
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsMouseButtonPressedImpl(const int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const int state = glfwGetKey(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return {(float)xpos, (float)ypos};
	}

	bool WindowsInput::IsKeyPressedImpl(const int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
}
