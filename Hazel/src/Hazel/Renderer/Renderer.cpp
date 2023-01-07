#include "pch.h"

#include "Renderer.h"

#include "RenderCommand.h"

namespace Hazel
{
	void Renderer::BeginScene() {}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::EndScene() {}
}
