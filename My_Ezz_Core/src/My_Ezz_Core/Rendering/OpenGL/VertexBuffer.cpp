#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"

#include "My_Ezz_Core/Log.hpp"

#include "glad/glad.h"
using namespace My_Ezz;

constexpr unsigned int shaderDataTypeToComponentsCount(const ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
	case ShaderDataType::Int:
		return 1;

	case ShaderDataType::Float2:
	case ShaderDataType::Int2:
		return 2;

	case ShaderDataType::Float3:
	case ShaderDataType::Int3:
		return 3;

	case ShaderDataType::Float4:
	case ShaderDataType::Int4:
		return 4;
	}

	LOG_ERROR("shaderDataTypeToComponentsCount: unknown ShaderDataType!");
	return 0;
}

constexpr size_t shaderDataTypeSize(const ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
		return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);

	case ShaderDataType::Int:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4:
		return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
	}
	LOG_ERROR("shaderDataTypeSize: unknown ShaderDataType!");
	return 0;
}

constexpr unsigned int shaderDataTypeToComponentType(const ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:
	case ShaderDataType::Float2:
	case ShaderDataType::Float3:
	case ShaderDataType::Float4:
		return GL_FLOAT;

	case ShaderDataType::Int:
	case ShaderDataType::Int2:
	case ShaderDataType::Int3:
	case ShaderDataType::Int4:
		return GL_INT;
	}

	LOG_ERROR("shaderDataTypeToComponentType: unknown ShaderDataType!");
	return GL_FLOAT;
}

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

BufferElement::BufferElement(const ShaderDataType _type)
	: type(_type),
	componentType(shaderDataTypeToComponentType(_type)),
	componentsCount(shaderDataTypeToComponentsCount(_type)),
	size(shaderDataTypeSize(_type)),
	offset(0)
{

}

VertexBuffer::VertexBuffer(const void* _data, const size_t _size, BufferLayout _bufferLayout, const EUsage _usage)
	: m_bufferLayout(std::move(_bufferLayout))
{
	glGenBuffers(1, &m_id);
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, _size, _data, usageToGLenum(_usage));
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_id);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& _vertexBuffer) noexcept
{
	m_id = _vertexBuffer.m_id;
	_vertexBuffer.m_id = 0;
	return *this;
}

VertexBuffer::VertexBuffer(VertexBuffer&& _vertexBuffer) noexcept
	: m_bufferLayout(std::move(_vertexBuffer.m_bufferLayout)),
	m_id(_vertexBuffer.m_id)
{
	_vertexBuffer.m_id = 0;
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}


void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
