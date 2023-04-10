#pragma once
#include <vector>
namespace My_Ezz
{

	enum class ShaderDataType
	{
		Float,
		Float2,
		Float3,
		Float4,
		Int,
		Int2, 
		Int3,
		Int4
	};

	struct BufferElement
	{
		ShaderDataType type;
		uint32_t componentType;
		size_t componentsCount;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType _type);
	};

	class BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(std::move(elements))
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements)
			{
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}
		const std::vector<BufferElement> getElements() const { return m_elements; }
		size_t getStride() const { return m_stride; }
	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};

	class VertexBuffer
	{
	public:

		enum class EUsage
		{
			Static,
			Dynamic,
			Stream
		};

		VertexBuffer(const void* _data, const size_t _size, BufferLayout _bufferLayout, const EUsage _usage = VertexBuffer::EUsage::Static);
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&& _vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& _vertexBuffer) noexcept;

		uint64_t GetHandle() const { return m_id; }

		const BufferLayout& getLayout() const { return m_bufferLayout; }
	private:
		unsigned int m_id = 0;
		BufferLayout m_bufferLayout;
	};
}