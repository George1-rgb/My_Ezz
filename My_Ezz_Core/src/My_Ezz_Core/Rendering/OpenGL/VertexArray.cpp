#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Logger/Log.hpp"
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
	for (const BufferElement& currentElement : _vertexBuffer.getLayout().getElements())
	{
		glEnableVertexAttribArray(m_countOfElements);
		
		glBindVertexBuffer(m_countOfElements, _vertexBuffer.GetHandle(), 
						   currentElement.offset,
						   static_cast<GLsizei>(_vertexBuffer.getLayout().getStride()));

		glVertexAttribFormat(m_countOfElements, 
							 static_cast<GLint>(currentElement.componentsCount), 
							 currentElement.componentType, 
							 GL_FALSE,
							 0);

		glVertexAttribBinding(m_countOfElements, m_countOfElements);

		++m_countOfElements;
	}
}

void VertexArray::setIndexBuffer(const IndexBuffer& _indexBuffer)
{
	bind();
	_indexBuffer.bind();
	m_indecesCount = _indexBuffer.getCount();
	unbind();
	_indexBuffer.unbind();
}

void VertexArray::bind() const
{
	glBindVertexArray(m_id);
}

void VertexArray::unbind()
{
	
	glBindVertexArray(0);
}