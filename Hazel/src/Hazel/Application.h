#pragma once

#include "Core.h"
#include "Hazel/Core/Timestep.h"
#include "Window.h"
#include "Hazel/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/ImGui/ImGuiLayer.h"

namespace Hazel
{
	class HAZEL_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		[[nodiscard]] Window& GetWindow() const { return *m_Window; }

		[[nodiscard]] static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& event);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTimePoint;
		Timestep m_LastFrameTimestep;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}
