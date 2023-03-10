#include "pch.h"

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> Shader; // Do both Texture and FlatColor
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data == nullptr,
		               "Hazel::Renderer2D::Init() was called when there was already an instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Shutdown() first to delete this instance."
		);

		s_Data = new Renderer2DStorage();

		/* VertexBuffer */
		constexpr float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};
		const Ref<VertexBuffer> squareVB = VertexBuffer::Create(squareVertices, sizeof squareVertices / sizeof(float));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TextureCoordinates"}
		});

		/* IndexBuffer */
		constexpr uint32_t squareIndices[6]{0, 1, 2, 2, 3, 0};
		const Ref<IndexBuffer> squareIB = IndexBuffer::Create(squareIndices, sizeof squareIndices / sizeof(uint32_t));

		s_Data->QuadVertexArray = VertexArray::Create();
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

		s_Data->Shader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->Shader->Bind();
		s_Data->Shader->SetInt("u_Texture", 0);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		constexpr uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::Shutdown() was called when there was no instance of Hazel::Renderer2DStorage existed."
		);

		delete s_Data;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::BeginScene() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		s_Data->Shader->Bind();
		s_Data->Shader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		HZ_PROFILE_FUNCTION();
	}

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
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		s_Data->QuadVertexArray->Bind();

		const glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position)
		                            * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->Shader->SetFloat4("u_Color", color);
		s_Data->Shader->SetFloat("u_TilingFactor", 1.0f);

		s_Data->WhiteTexture->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingFactor)
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		DrawQuad({position.x, position.y, 0.0f}, size, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		s_Data->QuadVertexArray->Bind();

		const glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position)
		                            * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->Shader->SetFloat4("u_Color", glm::vec4{1.0f});
		s_Data->Shader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawRotatedQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& color)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawRotatedQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		s_Data->QuadVertexArray->Bind();

		const glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position)
		                            * glm::rotate(glm::mat4{1.0f}, rotation, {0.0f, 0.0f, 1.0f})
		                            * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->Shader->SetFloat4("u_Color", color);
		s_Data->Shader->SetFloat("u_TilingFactor", 1.0f);

		s_Data->WhiteTexture->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture,
	                                 const float tilingFactor)
	{
		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawRotatedQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		DrawRotatedQuad({position.x, position.y, 0.0f}, size, rotation, texture, tilingFactor);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture,
	                                 const float tilingFactor)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(s_Data != nullptr,
		               "Hazel::Renderer2D::DrawQuad() was called when there was no instance of Hazel::Renderer2DStorage existed. "
		               "Call Hazel::Renderer2D::Init() first to initialize such an instance."
		);

		s_Data->QuadVertexArray->Bind();

		const glm::mat4 transform = glm::translate(glm::mat4{1.0f}, position)
		                            * glm::rotate(glm::mat4{1.0f}, rotation, {0.0f, 0.0f, 1.0f})
		                            * glm::scale(glm::mat4{1.0f}, {size.x, size.y, 1.0f});
		s_Data->Shader->SetMat4("u_Transform", transform);

		s_Data->Shader->SetFloat4("u_Color", glm::vec4{1.0f});
		s_Data->Shader->SetFloat("u_TilingFactor", tilingFactor);

		texture->Bind();

		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}
