#pragma once
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
namespace My_Ezz
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& _vertexArray) noexcept;
		VertexArray(VertexArray&& _vertexArray) noexcept;

		void addVertexBuffer(const VertexBuffer& _vertexBuffer);
		void setIndexBuffer(const IndexBuffer& _indexBuffer);
		size_t getIndecesCount() const { return m_indecesCount; }

		void bind() const;
		static void unbind();
	private:
		unsigned int m_id = 0;
		unsigned int m_countOfElements = 0;
		size_t m_indecesCount;
	};
}