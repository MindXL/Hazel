#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel
{
	class OpenGLVertexBuffer final : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float* vertices, uint32_t count);
		~OpenGLVertexBuffer() override;

		void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		[[nodiscard]] const BufferLayout& GetLayout() const override { return m_Layout; }

		void Bind() const override;
		void Unbind() const override;

	private:
		uint32_t m_RendererID{};
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer final : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer() override;

		void Bind() const override;
		void Unbind() const override;

		[[nodiscard]] uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_RendererID{};
		uint32_t m_Count;
	};
}
