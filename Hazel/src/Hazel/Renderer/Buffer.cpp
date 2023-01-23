#include "pch.h"

#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Hazel
{
	Ref<VertexBuffer> VertexBuffer::Create(const float* vertices, const uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			{
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, count);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, const uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			{
				HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
		case RendererAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		: m_Elements{elements}
	{
		CalculateOffsetAndStride();
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (BufferElement& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}
}
