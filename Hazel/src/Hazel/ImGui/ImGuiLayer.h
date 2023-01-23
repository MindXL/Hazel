#pragma once

#include "Hazel/Core/Layer.h"

namespace Hazel
{
	class HAZEL_API ImGuiLayer final : public Layer
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
