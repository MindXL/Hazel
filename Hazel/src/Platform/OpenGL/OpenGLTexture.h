#pragma once

#include "Hazel/Renderer/Texture.h"

#include <glad/gl.h>

namespace Hazel
{
	class OpenGLTexture2D final : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		explicit OpenGLTexture2D(std::string path);
		~OpenGLTexture2D() override;

		[[nodiscard]] uint32_t GetWidth() const override { return m_Width; }
		[[nodiscard]] uint32_t GetHeight() const override { return m_Height; }

		void SetData(const void* data, uint32_t size) const override;

		void Bind(uint32_t slot = 0) const override;

	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID{};
		GLenum m_InternalFormat = 0, m_DataFormat = 0;
	};
}
