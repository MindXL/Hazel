#include "pch.h"

#include "Application.h"

#include "Hazel/Log.h"
#include "Input.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		HZ_CORE_ASSERT(s_Instance == nullptr, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		{
			/* VertexBuffer */
			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};
			std::shared_ptr<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices) / sizeof(float)));
			vertexBuffer->SetLayout({
				{ ShaderDataType::Float3, "a_Potision" },
				{ ShaderDataType::Float4, "a_Color" }
				});

			/* IndexBuffer */
			uint32_t indices[3]{ 0, 1, 2 };
			std::shared_ptr<IndexBuffer> indexBuffer;
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

			/* VertexArray */
			m_VertexArray.reset(VertexArray::Create());
			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);

			std::string vertexSource = R"(
				#version 460 core

				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				out vec3 v_Position;
				out vec4 v_Color;

				void main()
				{
					v_Position = a_Position;
					v_Color = a_Color;
					gl_Position = vec4(a_Position, 1.0);
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
			m_Shader = std::make_shared<Shader>(vertexSource, fragmentSource);
		}

		{
			/* VertexBuffer */
			float squareVertices[3 * 4] = {
				-0.75f, -0.75f, 0.0f,
				 0.75f, -0.75f, 0.0f,
				 0.75f,  0.75f, 0.0f,
				-0.75f,  0.75f, 0.0f,
			};
			std::shared_ptr<VertexBuffer> squareVB;
			squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices) / sizeof(float)));
			squareVB->SetLayout({
					{ ShaderDataType::Float3, "a_Potision" }
				});

			/* IndexBuffer */
			uint32_t squareIndices[6]{ 0, 1, 2, 2, 3, 0 };
			std::shared_ptr<IndexBuffer> squareIB;
			squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));

			m_SquareVA.reset(VertexArray::Create());
			m_SquareVA->AddVertexBuffer(squareVB);
			m_SquareVA->SetIndexBuffer(squareIB);

			std::string blueShaderVertexSource = R"(
				#version 460 core

				layout(location = 0) in vec3 a_Position;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;
					gl_Position = vec4(a_Position, 1.0);
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
			m_BlueShader = std::make_shared<Shader>(blueShaderVertexSource, blueShaderFragmentSource);
		}
	}

	void Application::Run()
	{
		while (m_Running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			Renderer::BeginScene();

			m_BlueShader->Bind();
			Renderer::Submit(m_SquareVA);

			m_Shader->Bind();
			Renderer::Submit(m_VertexArray);

			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher{ event };
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

		for (auto rit = m_LayerStack.rbegin(); rit != m_LayerStack.rend(); rit++)
		{
			(*rit)->OnEvent(event);
			if (event.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}
}
