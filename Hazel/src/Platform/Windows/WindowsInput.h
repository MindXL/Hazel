#pragma once

#include "Hazel/Input.h"

namespace Hazel
{
	class WindowsInput :public Input
	{
	protected:
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		inline virtual float GetMouseXImpl() override { return GetMousePositionImpl().first; }
		inline virtual float GetMouseYImpl() override { return GetMousePositionImpl().second; }

		virtual bool IsKeyPressedImpl(int keycode) override;
	};
}
