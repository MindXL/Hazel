#include "pch.h"

#include "Renderer.h"

#include "RenderCommand.h"

namespace Hazel
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData();

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
	{
		vertexArray->Bind();

		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", s_SceneData->ViewProjectionMatrix);
		shader->UploadUniformMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene() {}
}
