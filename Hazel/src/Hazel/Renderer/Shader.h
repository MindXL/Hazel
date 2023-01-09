#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
	class Shader
	{
	public:
		/*
		* @param vertexSource: source code for vertex shader.
		* @param fragmentSource: source code for fragment shader.
		*/
		Shader(const std::string& vertexSource, const std::string& fragmentSource);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		uint32_t m_RendererID;
	};
}
