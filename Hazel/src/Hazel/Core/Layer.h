#pragma once

#include "Core.h"
#include "Timestep.h"
#include "Hazel/Events/Event.h"

namespace Hazel
{
	class Layer
	{
	public:
		explicit Layer(std::string debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		[[nodiscard]] const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
