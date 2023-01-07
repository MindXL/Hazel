#pragma once

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

	private:
		uint32_t m_RendererID;
	};
}
