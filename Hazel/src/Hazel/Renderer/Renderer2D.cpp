#include "pch.h"

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		HZ_CORE_ASSERT(s_Data == nullptr,
		               "Hazel::Renderer2D::Init() was called when there was already an instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Shutdown() first to delete this instance."
		);

		s_Data = new Renderer2DStorage();

		/* VertexBuffer */
		constexpr float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f
		};
		const Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof squareVertices / sizeof(float));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
		});

		/* IndexBuffer */
		constexpr uint32_t squareIndices[6]{0, 1, 2, 2, 3, 0};
		const Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof squareIndices / sizeof(uint32_t));

		s_Data->QuadVertexArray = VertexArray::Create();
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}

	void Renderer2D::Shutdown()
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::Shutdown() was called when there was no instance of Hazel::Renderer2DStorage existed."
		);

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::BeginScene() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4(
			"u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4(
			"u_Transform", glm::mat4{1.0f});
	}

	void Renderer2D::EndScene() {}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		DrawQuad({position.x, position.y, 0.0f}, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();

		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}
