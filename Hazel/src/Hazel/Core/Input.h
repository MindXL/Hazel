#pragma once

#include "Core.h"

namespace Hazel
{
	class Input
	{
	public:
		virtual ~Input() = default;

		static bool IsMouseButtonPressed(const int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

		static bool IsKeyPressed(const int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

	protected:
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

		virtual bool IsKeyPressedImpl(int keycode) = 0;

	private:
		static Scope<Input> s_Instance;
	};
}
