#pragma once

#include <glm/glm.hpp>

namespace Hazel
{
	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetInt(const std::string& name, int value) const = 0;
		virtual void SetFloat(const std::string& name, float value) const = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) const = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) const = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) const = 0;
		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) const = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) const = 0;

		[[nodiscard]] virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(std::string name, const std::string& filepath);
		static Ref<Shader> Create(std::string name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		inline void Add(const Ref<Shader>& shader);
		Ref<Shader> Load(std::string name, const std::string& filepath);
		Ref<Shader> Load(const std::string& filepath);

		// TODO: Causes link error if declaration of Get() is put in Shader.cpp.
		//[[nodiscard]] inline Ref<Shader> Get(const std::string& name);
		[[nodiscard]] Ref<Shader> Get(const std::string& name)
		{
			HZ_CORE_ASSERT(Exists(name), "Shader doesn't exist.");
			return m_Shaders.at(name);
		}

		[[nodiscard]] bool Exists(const std::string& name) const { return m_Shaders.contains(name); }

	private:
		std::unordered_map<std::string_view, Ref<Shader>> m_Shaders;
	};
}
