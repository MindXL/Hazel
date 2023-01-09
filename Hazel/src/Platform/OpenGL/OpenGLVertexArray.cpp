#include "pch.h"

#include "OpenGLVertexArray.h"

#include <glad/gl.h>

namespace Hazel
{
	static GLenum ShaderDataTypeToGLenum(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Int:
		case Hazel::ShaderDataType::Int2:
		case Hazel::ShaderDataType::Int3:
		case Hazel::ShaderDataType::Int4: return GL_INT;
		case Hazel::ShaderDataType::Float:
		case Hazel::ShaderDataType::Float2:
		case Hazel::ShaderDataType::Float3:
		case Hazel::ShaderDataType::Float4: return GL_FLOAT;
		case Hazel::ShaderDataType::Bool: return GL_BOOL;
		case Hazel::ShaderDataType::Mat3:
		case Hazel::ShaderDataType::Mat4: return GL_FLOAT;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size() != 0, "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const BufferLayout& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const BufferElement& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.Count,
				ShaderDataTypeToGLenum(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(void*)element.Offset
			);
			index++;
		}

		m_VertexBuffers.emplace_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
