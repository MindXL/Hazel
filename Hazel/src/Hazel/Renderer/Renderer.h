#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace Hazel
{
	class Renderer
	{
	public:
		static void BeginScene(const OrthographicCamera& camera);
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader,
		                   const glm::mat4& transform = glm::mat4{1.0f});
		static void EndScene();

		[[nodiscard]] static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}
