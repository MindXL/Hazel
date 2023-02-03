#include "pch.h"

#include "OpenGLShader.h"

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{
	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		const std::string source{ReadFile(filepath)};
		const auto shaderSources{PreProcess(source)};
		Compile(shaderSources);

		// Extract name from filepath.
		const size_t lastSlash = filepath.find_last_of("/\\");
		const size_t nameBegin = lastSlash != std::string::npos ? lastSlash + 1 : 0;
		const size_t lastDot = filepath.rfind('.');
		const size_t count = lastDot != std::string::npos ? lastDot - lastSlash : filepath.length() - lastSlash;
		m_Name = filepath.substr(nameBegin, count);
	}

	OpenGLShader::OpenGLShader(std::string name, const std::string& filepath)
		: m_Name{std::move(name)}
	{
		const std::string source{ReadFile(filepath)};
		const auto shaderSources{PreProcess(source)};
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(std::string name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_Name{std::move(name)}
	{
		Compile({{GL_VERTEX_SHADER, vertexSource}, {GL_FRAGMENT_SHADER, fragmentSource}});
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, const int value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, const float value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& matrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& matrix) const
	{
		const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::ifstream in{filepath, std::ios::in | std::ios::binary};
		if (in.fail())
			HZ_CORE_ERROR("Could not open file \"{0}\"", filepath);

		std::string result;

		in.seekg(0, std::ios::end);
		result.resize(in.tellg());

		in.seekg(0, std::ios::beg);
		in.read(result.data(), (long long)result.size());

		in.close();

		return result;
	}

	static GLenum ShaderTypeFromString(const std::string_view& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const std::string typeToken = "#type";
		const size_t typeTokenLength = typeToken.length();

		for (size_t pos = source.find(typeToken); pos != std::string::npos;)
		{
			const size_t eol = source.find_first_of("\r\n", pos);
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error.");

			const size_t begin = pos + typeTokenLength + 1;
			std::string_view typeString{source.data() + begin, eol - begin};
			GLenum type = ShaderTypeFromString(typeString);
			HZ_CORE_ASSERT(type != 0, "Invalid shader type specified.");

			const size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			HZ_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");

			pos = source.find(typeToken, nextLinePos);
			if (pos != std::string::npos)
				shaderSources.emplace(type, source.substr(nextLinePos, pos - nextLinePos));
			else
				shaderSources.emplace(type, source.substr(nextLinePos));
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		static constexpr size_t MAX_SHADER_NUM = 2;

		const GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= MAX_SHADER_NUM, "We don't support that many shaders for now.");

		std::array<GLenum, MAX_SHADER_NUM> glShaderIDs{};
		int glShaderIDIndex = 0;

		for (const auto& [type, source] : shaderSources)
		{
			const GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, nullptr);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

				glDeleteShader(shader);

				HZ_CORE_ASSERT(false, "Shader compilation failed:");
				HZ_CORE_ERROR("{0}", infoLog.data());
				break;
			}

			glAttachShader(program, shader);

			glShaderIDs[glShaderIDIndex] = shader;
			glShaderIDIndex++;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());

			glDeleteProgram(program);
			for (const auto& id : glShaderIDs)
				glDeleteShader(id);

			HZ_CORE_ASSERT(false, "Shader link failed:");
			HZ_CORE_ERROR("{0}", infoLog.data());
			return;
		}

		// Always detach shaders after a successful link.
		for (const auto& id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}

		m_RendererID = program;
	}
}
