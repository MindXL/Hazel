#pragma once

#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Hazel
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL
		};

		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

		[[nodiscard]] static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}
