#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <ImGui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

class ExampleLayer final : public Hazel::Layer
{
public:
	ExampleLayer()
		: Layer{"Example"}, m_CameraController{1280.0f / 720.0f, true}
	{
		{
			/* VertexBuffer */
			constexpr float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};
			const Hazel::Ref<Hazel::VertexBuffer> vertexBuffer = Hazel::VertexBuffer::Create(
				vertices, sizeof vertices / sizeof(float));
			vertexBuffer->SetLayout({
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float4, "a_Color"}
			});

			/* IndexBuffer */
			constexpr uint32_t indices[3]{0, 1, 2};
			const Hazel::Ref<Hazel::IndexBuffer> indexBuffer = Hazel::IndexBuffer::Create(
				indices, sizeof indices / sizeof(uint32_t));

			/* VertexArray */
			m_VertexArray = Hazel::VertexArray::Create();
			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);

			const std::string vertexSource = R"(
				#version 460 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjectionMatrix;
				uniform mat4 u_Transform;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
				}
			)";
			const std::string fragmentSource = R"(
				#version 460 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = v_Color;
				}
			)";
			m_Shader = Hazel::Shader::Create("VertexPositionColor", vertexSource, fragmentSource);
		}

		{
			/* VertexBuffer */
			constexpr float squareVertices[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
			};
			const Hazel::Ref<Hazel::VertexBuffer> squareVB = Hazel::VertexBuffer::Create(
				squareVertices, sizeof squareVertices / sizeof(float));
			squareVB->SetLayout({
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float2, "a_TextureCoordinates"}
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

		const auto textureShader = m_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep timestep) override
	{
		// Update
		m_CameraController.OnUpdate(timestep);

		// Render
		Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
		Hazel::RenderCommand::Clear();

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		const glm::mat4 scale = glm::scale(glm::mat4{1.0f}, glm::vec3{0.1f});

		m_FlatColorShader->Bind();
		m_FlatColorShader->SetFloat4("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 position{x * 0.11f, y * 0.11f, 0.0f};
				glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position) * scale;
				Hazel::Renderer::Submit(m_SquareVA, m_FlatColorShader, transform);
			}
		}

		const auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Hazel::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));

		m_ChernoLogoTexture->Bind();
		Hazel::Renderer::Submit(m_SquareVA, textureShader, glm::scale(glm::mat4{1.0f}, glm::vec3{1.5f}));

		//Hazel::Renderer::Submit(m_VertexArray, m_Shader);

		Hazel::Renderer::EndScene();
	}

	void OnEvent(Hazel::Event& event) override
	{
		m_CameraController.OnEvent(event);
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

private:
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::Shader> m_Shader;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoLogoTexture;

	Hazel::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};

class Sandbox final : public Hazel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
