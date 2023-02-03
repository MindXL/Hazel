#pragma once

#include "Hazel/Renderer/Shader.h"

#include <glm/glm.hpp>

using GLenum = unsigned int;

namespace Hazel
{
	class OpenGLShader final : public Shader
	{
	public:
		explicit OpenGLShader(const std::string& filepath);
		OpenGLShader(std::string name, const std::string& filepath);
		/*
		* @param vertexSource: source code for vertex shader.
		* @param fragmentSource: source code for fragment shader.
		*/
		OpenGLShader(std::string name, const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader() override;

		void Bind() const override;
		void Unbind() const override;

		void SetInt(const std::string& name, int value) const override;
		void SetFloat(const std::string& name, float value) const override;
		void SetFloat2(const std::string& name, const glm::vec2& value) const override;
		void SetFloat3(const std::string& name, const glm::vec3& value) const override;
		void SetFloat4(const std::string& name, const glm::vec4& value) const override;
		void SetMat3(const std::string& name, const glm::mat3& matrix) const override;
		void SetMat4(const std::string& name, const glm::mat4& matrix) const override;

		[[nodiscard]] const std::string& GetName() const override { return m_Name; }

	private:
		static std::string ReadFile(const std::string& filepath);
		static std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID{};
		std::string m_Name;
	};
}
