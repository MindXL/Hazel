#pragma once

#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

namespace Hazel
{
	class OpenGLShader final : public Shader
	{
	public:
		/*
		* @param vertexSource: source code for vertex shader.
		* @param fragmentSource: source code for fragment shader.
		*/
		OpenGLShader(const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void UploadUniformInt(const std::string& name, const int value) const;

		void UploadUniformFloat(const std::string& name, const float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

	private:
		uint32_t m_RendererID{};
	};
}
