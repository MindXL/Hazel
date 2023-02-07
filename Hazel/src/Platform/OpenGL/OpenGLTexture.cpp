#include "pch.h"

#include "OpenGLTexture.h"

#include <glad/gl.h>
#include <stb_image.h>

namespace Hazel
{
	OpenGLTexture2D::OpenGLTexture2D(const uint32_t width, const uint32_t height)
		: m_Width{width}, m_Height{height}, m_InternalFormat{GL_RGBA8}, m_DataFormat{GL_RGBA}
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, (int)m_Width, (int)m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	OpenGLTexture2D::OpenGLTexture2D(std::string path)
		: m_Path{std::move(path)}
	{
		int width = 0, height = 0, channels = 0;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(data, "Failed to load image!");

		m_Width = (uint32_t)width;
		m_Height = (uint32_t)height;

		if (channels == 3)
		{
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		HZ_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, (int)m_Width, (int)m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, (int)m_Width, (int)m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(const void* data, const uint32_t size) const
	{
#ifdef HZ_ENABLE_ASSERTS
		const uint32_t bytesPerPixel = m_DataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == m_Width * m_Height * bytesPerPixel, "Data must be the entire texture.");
#endif // HZ_ENABLE_ASSERTS

		glTextureSubImage2D(m_RendererID, 0, 0, 0, (int)m_Width, (int)m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(const uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}
