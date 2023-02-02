﻿#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <ImGui/imgui.h>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer{"Sandbox2D"}, m_CameraController{1280.0f / 720.0f, true} {}

void Sandbox2D::OnAttach()
{
	/* VertexBuffer */
	constexpr float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};
	const Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(
		squareVertices, sizeof squareVertices / sizeof(float));
	squareVB->SetLayout({
		{Hazel::ShaderDataType::Float3, "a_Position"},
	});

	/* IndexBuffer */
	constexpr uint32_t squareIndices[6]{0, 1, 2, 2, 3, 0};
	const Hazel::Ref<Hazel::IndexBuffer> squareIB = Hazel::IndexBuffer::Create(
		squareIndices, sizeof squareIndices / sizeof(uint32_t));

	m_SquareVA = Hazel::VertexArray::Create();
	m_SquareVA->AddVertexBuffer(squareVB);
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnUpdate(Hazel::Timestep timestep)
{
	// Update
	m_CameraController.OnUpdate(timestep);

	// Render
	Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

	// TODO: Causes binding shader twice.
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Hazel::Renderer::Submit(m_SquareVA, m_FlatColorShader, glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));

	Hazel::Renderer::EndScene();
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
