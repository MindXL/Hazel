#pragma once

#include "Hazel/Input.h"

namespace Hazel
{
	class WindowsInput final : public Input
	{
	protected:
		bool IsMouseButtonPressedImpl(const int button) override;
		[[nodiscard]] std::pair<float, float> GetMousePositionImpl() override;
		[[nodiscard]] float GetMouseXImpl() override { return GetMousePositionImpl().first; }
		[[nodiscard]] float GetMouseYImpl() override { return GetMousePositionImpl().second; }

		[[nodiscard]] bool IsKeyPressedImpl(const int keycode) override;
	};
}
