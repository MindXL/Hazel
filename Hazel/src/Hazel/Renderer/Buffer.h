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

	static uint32_t ShaderDataTypeCount(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None: break;
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Int2: return 2;
		case ShaderDataType::Int3: return 3;
		case ShaderDataType::Int4: return 4;
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::Mat3: return 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!")
		return 0;
	}

	static uint32_t ShaderDataTypeSize(const ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::None: break;
		case ShaderDataType::Int: return sizeof(int);
		case ShaderDataType::Int2: return 2 * sizeof(int);
		case ShaderDataType::Int3: return 3 * sizeof(int);
		case ShaderDataType::Int4: return 4 * sizeof(int);
		case ShaderDataType::Float: return sizeof(float);
		case ShaderDataType::Float2: return 2 * sizeof(float);
		case ShaderDataType::Float3: return 3 * sizeof(float);
		case ShaderDataType::Float4: return 4 * sizeof(float);
		case ShaderDataType::Bool: return sizeof(bool);
		case ShaderDataType::Mat3: return 3 * 3 * sizeof(float);
		case ShaderDataType::Mat4: return 4 * 4 * sizeof(float);
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!")
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

		BufferElement(const ShaderDataType type, std::string name, const bool normalized = false)
			: Type{type},
			  Name{std::move(name)},
			  Count{ShaderDataTypeCount(type)},
			  Size{ShaderDataTypeSize(type)},
			  Offset{0},
			  Normalized{normalized} {}
	};

	class BufferLayout
	{
	public:
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>& elements);

		[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		[[nodiscard]] uint32_t GetStride() const { return m_Stride; }

		[[nodiscard]] std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_Elements.cbegin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_Elements.cend(); }

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
		[[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;

		// TODO: Is this GetCount necessary?
		// virtual uint32_t GetCount() const = 0;

		static VertexBuffer* Create(const float* vertices, const uint32_t count);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		[[nodiscard]] virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(const uint32_t* indices, const uint32_t count);
	};
}
