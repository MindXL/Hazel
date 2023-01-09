#include <Hazel.h>

#include <ImGui/imgui.h>

class ExampleLayer :public Hazel::Layer
{
public:
	ExampleLayer()
		:Layer{ "Example" }, m_Camera{ -1.6f, 1.6f, -0.9f, 0.9f }, m_CameraPosition{ 0.0f }
	{
		{
			/* VertexBuffer */
			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};
			std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));
			vertexBuffer->SetLayout({
				{ Hazel::ShaderDataType::Float3, "a_Potision" },
				{ Hazel::ShaderDataType::Float4, "a_Color" }
				});

			/* IndexBuffer */
			uint32_t indices[3]{ 0, 1, 2 };
			std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
			indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

			/* VertexArray */
			m_VertexArray.reset(Hazel::VertexArray::Create());
			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);

			std::string vertexSource = R"(
				#version 460 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjectionMatrix;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
				}
			)";
			std::string fragmentSource = R"(
				#version 460 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;
				in vec4 v_Color;

				void main()
				{
					color = v_Color;
				}
			)";
			m_Shader = std::make_shared<Hazel::Shader>(vertexSource, fragmentSource);
		}

		{
			/* VertexBuffer */
			float squareVertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f,
			};
			std::shared_ptr<Hazel::VertexBuffer> squareVB;
			squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices) / sizeof(float)));
			squareVB->SetLayout({
					{ Hazel::ShaderDataType::Float3, "a_Potision" }
				});

			/* IndexBuffer */
			uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<Hazel::IndexBuffer> squareIB;
			squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

			m_SquareVA.reset(Hazel::VertexArray::Create());
			m_SquareVA->AddVertexBuffer(squareVB);
			m_SquareVA->SetIndexBuffer(squareIB);

			std::string blueShaderVertexSource = R"(
				#version 460 core

				layout(location = 0) in vec3 a_Position;

				uniform mat4 u_ViewProjectionMatrix;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
				}
			)";
			std::string blueShaderFragmentSource = R"(
				#version 460 core

				layout(location = 0) out vec4 color;

				in vec3 v_Position;

				void main()
				{
					color = vec4(0.2, 0.3, 0.8, 1.0);
				}
			)";
			m_BlueShader = std::make_shared<Hazel::Shader>(blueShaderVertexSource, blueShaderFragmentSource);
		}
	}

	virtual void OnUpdate(Hazel::Timestep timestep) override
	{
		const float time = timestep.GetSeconds();

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * time;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * time;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * time;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * time;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * time;
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * time;


		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Hazel::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_Camera);

		Hazel::Renderer::Submit(m_SquareVA, m_BlueShader);
		Hazel::Renderer::Submit(m_VertexArray, m_Shader);

		Hazel::Renderer::EndScene();
	}

	virtual void OnEvent(Hazel::Event& event) override {}

	virtual void OnImGuiRender() override {}

private:
	std::shared_ptr<Hazel::VertexArray> m_VertexArray;
	std::shared_ptr<Hazel::Shader> m_Shader;

	std::shared_ptr<Hazel::VertexArray> m_SquareVA;
	std::shared_ptr<Hazel::Shader> m_BlueShader;

	Hazel::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotation = 0.0f;
	float m_CameraRotationSpeed = 180.0f;
};

class Sandbox :public Hazel::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}
