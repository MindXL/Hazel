#include "Sandbox2D.h"

#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer{"Sandbox2D"},
	  m_CheckerboardTexture{Hazel::Texture2D::Create("assets/textures/Checkerboard.png")},
	  m_CameraController{1280.0f / 720.0f, true} {}

void Sandbox2D::OnAttach() {}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep)
{
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

	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
	m_CameraController.OnEvent(event);
}
