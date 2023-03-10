#include "pch.h"

#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Hazel
{
	template<typename... Args>
	static Ref<Shader> InnerCreate(Args&&... args)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			{
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(std::forward<Args>(args)...);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		return InnerCreate(filepath);
	}

	Ref<Shader> Shader::Create(std::string name, const std::string& filepath)
	{
		return InnerCreate(std::move(name), filepath);
	}

	Ref<Shader> Shader::Create(std::string name,
	                           const std::string& vertexSource, const std::string& fragmentSource)
	{
		return InnerCreate(std::move(name), vertexSource, fragmentSource);
	}

	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		HZ_CORE_ASSERT(!Exists(shader->GetName()), "Shader already exists.");

		m_Shaders.emplace(shader->GetName(), shader);
	}

	Ref<Shader> ShaderLibrary::Load(std::string name, const std::string& filepath)
	{
		auto shader = Shader::Create(std::move(name), filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	// TODO: Results link error if declaration of Get() is put here. But no error respect to Add() above.
	//Ref<Shader> ShaderLibrary::Get(const std::string& name)
	//{
	//	HZ_CORE_ASSERT(Exists(name), "Shader doesn't exist.");
	//	return m_Shaders.at(name);
	//}
}
