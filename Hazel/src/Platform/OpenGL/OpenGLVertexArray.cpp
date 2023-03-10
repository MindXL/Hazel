#include "pch.h"

#include "OpenGLVertexArray.h"

#include <glad/gl.h>

namespace Hazel
{
	static GLenum ShaderDataTypeToGLenum(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None: break;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4: return GL_INT;
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Bool: return GL_BOOL;
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4: return GL_FLOAT;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		for (const BufferLayout& layout = vertexBuffer->GetLayout();
		     const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
			                      (int)element.Count,
			                      ShaderDataTypeToGLenum(element.Type),
			                      element.Normalized ? GL_TRUE : GL_FALSE,
			                      (int)layout.GetStride(),
			                      // TODO: Possible memory issue when 32-bit Offset cast to void* which has 64 bits?
			                      (void*)element.Offset);
			m_VertexBufferIndex++;
		}

		m_VertexBuffers.emplace_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
