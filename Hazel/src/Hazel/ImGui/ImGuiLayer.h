#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel
{
	class ImGuiLayer final : public Layer
	{
	public:
		ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;

		static void Begin();
		void OnImGuiRender() override;
		static void End();
	};
}
