#pragma once

#include <Hazel.h>

class Sandbox2D final : public Hazel::Layer
{
public:
	Sandbox2D();

	void OnAttach() override;

	void OnUpdate(Hazel::Timestep timestep) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& event) override;

private:
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = {0.8f, 0.2f, 0.3f, 1.0f};
};
