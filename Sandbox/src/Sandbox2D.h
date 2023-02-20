#pragma once

#include <Hazel.h>

class Sandbox2D final : public Hazel::Layer
{
public:
	Sandbox2D();

	void OnUpdate(Hazel::Timestep timestep) override;
	void OnImGuiRender() override;
	void OnEvent(Hazel::Event& event) override;

private:
	const Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = {0.8f, 0.2f, 0.3f, 1.0f};
};
