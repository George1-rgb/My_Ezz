#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Log.hpp"
#include <glad/glad.h>

using namespace My_Ezz;

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

VertexArray& VertexArray::operator=(VertexArray&& _vertexArray) noexcept
{
	m_id = _vertexArray.m_id;
	m_countOfElements = _vertexArray.m_countOfElements;

	_vertexArray.m_id = 0;
	_vertexArray.m_countOfElements = 0;
	return *this;
}


VertexArray::VertexArray(VertexArray&& _vertexArray) noexcept
{
	m_id = _vertexArray.m_id;
	m_countOfElements = _vertexArray.m_countOfElements;

	_vertexArray.m_id = 0;
	_vertexArray.m_countOfElements = 0;
}

void VertexArray::addVertexBuffer(const VertexBuffer& _vertexBuffer)
{
	bind();
	_vertexBuffer.bind();

	for (const BufferElement& currentElement : _vertexBuffer.getLayout().getElements())
	{
		glEnableVertexAttribArray(m_countOfElements);
		glVertexAttribPointer(
			m_countOfElements, 
			static_cast<GLint>(currentElement.componentsCount),
			currentElement.componentType,
			GL_FALSE, 
			static_cast<GLsizei>(_vertexBuffer.getLayout().getStride()),
			reinterpret_cast<const void*>(currentElement.offset)
		);
		++m_countOfElements;
	}
}

void VertexArray::setIndexBuffer(const IndexBuffer& _indexBuffer)
{
	bind();
	_indexBuffer.bind();
	m_indecesCount = _indexBuffer.getCount();


}

void VertexArray::bind() const
{
	glBindVertexArray(m_id);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}