#pragma once

namespace Hazel
{
	enum class ShaderDataType
	{
		None = 0,
		Int, Int2, Int3, Int4,
		Float, Float2, Float3, Float4,
		Bool,
		Mat3, Mat4
	};

	inline static uint32_t ShaderDataTypeCount(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Int: return 1;
		case Hazel::ShaderDataType::Int2: return 2;
		case Hazel::ShaderDataType::Int3: return 3;
		case Hazel::ShaderDataType::Int4: return 4;
		case Hazel::ShaderDataType::Float: return 1;
		case Hazel::ShaderDataType::Float2: return 2;
		case Hazel::ShaderDataType::Float3: return 3;
		case Hazel::ShaderDataType::Float4: return 4;
		case Hazel::ShaderDataType::Bool: return 1;
		case Hazel::ShaderDataType::Mat3: return 3 * 3;
		case Hazel::ShaderDataType::Mat4: return 4 * 4;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	inline static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case Hazel::ShaderDataType::Int: return sizeof(int);
		case Hazel::ShaderDataType::Int2: return 2 * sizeof(int);
		case Hazel::ShaderDataType::Int3: return 3 * sizeof(int);
		case Hazel::ShaderDataType::Int4: return 4 * sizeof(int);
		case Hazel::ShaderDataType::Float: return sizeof(float);
		case Hazel::ShaderDataType::Float2: return 2 * sizeof(float);
		case Hazel::ShaderDataType::Float3: return 3 * sizeof(float);
		case Hazel::ShaderDataType::Float4: return 4 * sizeof(float);
		case Hazel::ShaderDataType::Bool: return sizeof(bool);
		case Hazel::ShaderDataType::Mat3: return 3 * 3 * sizeof(float);
		case Hazel::ShaderDataType::Mat4: return 4 * 4 * sizeof(float);
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		ShaderDataType Type;
		std::string Name;
		uint32_t Count;
		uint32_t Size;
		uint32_t Offset;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			:Type{ type },
			Name{ name },
			Count{ ShaderDataTypeCount(type) },
			Size{ ShaderDataTypeSize(type) },
			Offset{ 0 },
			Normalized{ normalized } {}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.cbegin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.cend(); }

	private:
		void CalculateOffsetAndStride();

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		// TODO: Is this GetCount necessary?
		// virtual uint32_t GetCount() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indeices, uint32_t count);
	};
}
