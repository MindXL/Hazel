#include "pch.h"

#include "Renderer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
		HZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() {}

	void Renderer::Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& transform)
	{
		vertexArray->Bind();

		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::OnWindowResize(const uint32_t width, const uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}
}
