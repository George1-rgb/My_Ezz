#include "ShaderProgram.hpp"
#include "My_Ezz_Core/Log.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

using namespace My_Ezz;

bool create_shader(const char* _source, const GLenum _shaderType, GLuint& _shaderID)
{
	_shaderID = glCreateShader(_shaderType);
	glShaderSource(_shaderID, 1, &_source, nullptr);
	glCompileShader(_shaderID);

	GLint success;
	glGetShaderiv(_shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		char info_log[1024];
		glGetShaderInfoLog(_shaderID, 1024, nullptr, info_log);

		LOG_CRITICAL("Shader compilation error:\n", info_log);
		return false;
	}
	return true;
}

ShaderProgram::ShaderProgram(const char* _vertexShader_src, const char* _fragmentShader_src)
{
	GLuint vertexShaderID = 0;
	if (!create_shader(_vertexShader_src, GL_VERTEX_SHADER, vertexShaderID))
	{
		LOG_CRITICAL("VERTEX SHADER: compile-time error!");
		glDeleteShader(vertexShaderID);
		return;
	}

	GLuint fragmentShaderID = 0;
	if (!create_shader(_fragmentShader_src, GL_FRAGMENT_SHADER, fragmentShaderID))
	{
		LOG_CRITICAL("FRAGMENT SHADER: compile-time error!");
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return;
	}

	m_id = glCreateProgram();
	glAttachShader(m_id, vertexShaderID);
	glAttachShader(m_id, fragmentShaderID);
	glLinkProgram(m_id);

	GLint success;
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar info_log[1024];
		glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
		LOG_CRITICAL("SHADER PROGRAM: Link-time error:\n", info_log);
		glDeleteProgram(m_id);
		m_id = 0;
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		return;
	}
	else
	{
		m_isCompiled = true;
	}
	glDetachShader(m_id, vertexShaderID);
	glDetachShader(m_id, fragmentShaderID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram)
{
	m_id = shaderProgram.m_id;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_id = 0;
	shaderProgram, m_isCompiled = false;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram)
{
	glDeleteProgram(m_id);
	m_id = shaderProgram.m_id;
	m_isCompiled = shaderProgram.m_isCompiled;

	shaderProgram.m_id = 0;
	shaderProgram, m_isCompiled = false;

	return *this;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_id);
}

void ShaderProgram::bind() const
{
	glUseProgram(m_id);
}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}

void ShaderProgram::setMatrix4(const char* name, const glm::mat4& matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setInt(const char* name, const int nValue) const
{
	glUniform1i(glGetUniformLocation(m_id, name), nValue);
}

void ShaderProgram::setVec3(const char* name, const glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(m_id, name), value.x, value.y, value.z);
}

void ShaderProgram::setFloat(const char* name, const float value) const
{
	glUniform1f(glGetUniformLocation(m_id, name), value);
}

void ShaderProgram::setMatrix3(const char* name, const glm::mat3& matrix) const
{
	glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
