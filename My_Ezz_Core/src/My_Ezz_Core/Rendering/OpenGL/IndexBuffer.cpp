#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"

#include "My_Ezz_Core/Log.hpp"

#include "glad/glad.h"
using namespace My_Ezz;


constexpr GLenum usageToGLenum(const VertexBuffer::EUsage _usage)
{
	switch (_usage)
	{
	case VertexBuffer::EUsage::Static: return GL_STATIC_DRAW;
	case VertexBuffer::EUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case VertexBuffer::EUsage::Stream: return GL_STREAM_DRAW;
	}
	LOG_ERROR("Unknown VertexBuffer usage");
	return GL_STATIC_DRAW;
}


IndexBuffer::IndexBuffer(const void* _data, const size_t _count, const VertexBuffer::EUsage _usage)
	: m_count(_count)
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(GLuint), _data, usageToGLenum(_usage));
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& _indexBuffer) noexcept
{
	m_id = _indexBuffer.m_id;
	m_count - _indexBuffer.m_count;
	_indexBuffer.m_id = 0;
	_indexBuffer.m_count = 0;
	return *this;
}

IndexBuffer::IndexBuffer(IndexBuffer&& _indexBuffer) noexcept
	: m_id(_indexBuffer.m_id),
	m_count(_indexBuffer.m_count)
{
	_indexBuffer.m_id = 0;
	_indexBuffer.m_count = 0;
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}


void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
