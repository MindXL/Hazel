#pragma once

namespace Hazel
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		// TODO: Is this GetCount necessary?
		// virtual uint32_t GetCount() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indeices, uint32_t count);
	};
}
