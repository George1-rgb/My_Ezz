#pragma once
#include "VertexBuffer.hpp"
namespace My_Ezz
{

	class IndexBuffer
	{
	public:

		IndexBuffer(const void* _data, const size_t _count, const VertexBuffer::EUsage _usage = VertexBuffer::EUsage::Static);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&& _indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& _indexBuffer) noexcept;

		void bind() const;
		static void unbind();
		size_t getCount() const { return m_count; }
	private:
		unsigned int m_id = 0;
		size_t m_count;
	};
}