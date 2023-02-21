#include "pch.h"

#include "Application.h"

#include "Log.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Instance == nullptr, "Application already exists!");
		s_Instance = this;

		m_Window = Window::Create();
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		HZ_PROFILE_FUNCTION();

		Renderer2D::Shutdown();
	}

	void Application::Run()
	{
		HZ_PROFILE_FUNCTION();

		while (m_Running)
		{
			HZ_PROFILE_SCOPE("Run Loop");

			auto timePoint = std::chrono::steady_clock::now();
			Timestep timestep{timePoint - m_LastFrameTimePoint};
			m_LastFrameTimePoint = timePoint;
			m_LastFrameTimestep = timestep;

			if (!m_Minimized)
			{
				{
					HZ_PROFILE_SCOPE("Do every layer->OnUpdate() in LayerStack");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				ImGuiLayer::Begin();
				{
					HZ_PROFILE_SCOPE("Do every layer->OnImGuiRender() in LayerStack");
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				ImGuiLayer::End();
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		HZ_PROFILE_FUNCTION();

		EventDispatcher dispatcher{event};
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		for (const auto& rit : std::ranges::reverse_view(m_LayerStack))
		{
			rit->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		HZ_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		HZ_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowResize(const WindowResizeEvent& event)
	{
		HZ_PROFILE_FUNCTION();

		const uint32_t width = event.GetWidth(), height = event.GetHeight();

		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(width, height);

		return false;
	}

	bool Application::OnWindowClose(const WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}
