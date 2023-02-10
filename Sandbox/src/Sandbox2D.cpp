#include "Sandbox2D.h"

#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

class Timer
{
public:
	struct Provide
	{
		const char* Name;
		float Time;
	};

	Timer(const char* name, std::function<void(const Provide&)> callback)
		: m_Name{name}, m_StartTimepoint{std::chrono::steady_clock::now()}, m_Stopped{false}, m_Callback{std::move(callback)} {}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		const std::chrono::duration<float, std::milli> duration{std::chrono::steady_clock::now() - m_StartTimepoint};

		m_Stopped = true;

		m_Callback({.Name = m_Name, .Time = duration.count()});
	}

private:
	const char* m_Name;
	const std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
	std::function<void(const Provide&)> m_Callback;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__{name, [this](const Timer::Provide& provided) { m_ProfileResults.emplace_back(provided.Name, provided.Time); }}

Sandbox2D::Sandbox2D()
	: Layer{"Sandbox2D"},
	  m_CheckerboardTexture{Hazel::Texture2D::Create("assets/textures/Checkerboard.png")},
	  m_CameraController{1920.0f / 1080.0f, true} {}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate()");

	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, m_SquareColor);
	Hazel::Renderer2D::DrawQuad({1.0f, 0.0f}, {0.5f, 0.75f}, m_SquareColor);
	Hazel::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {5.0f, 5.0f}, m_CheckerboardTexture);

	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (const auto& result : m_ProfileResults)
		ImGui::Text("%.3f ms [%s]", static_cast<double>(result.Time), result.Name);

	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}
