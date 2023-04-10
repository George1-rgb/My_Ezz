#pragma once
#include <glm/mat4x4.hpp>
namespace My_Ezz
{
	class ShaderProgram
	{
	public:
		ShaderProgram(const char* _vertexShader_src, const char* _fragmentShader_src);
		ShaderProgram(ShaderProgram&&);
		ShaderProgram& operator=(ShaderProgram&&);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		void bind() const;
		static void unbind();
		bool isCompiled() const { return m_isCompiled; }
		void setMatrix4(const char* name, const glm::mat4& matrix) const;
		void setMatrix3(const char* name, const glm::mat3& matrix) const;
		void setInt(const char* name, const int nValue) const;
		void setVec3(const char* name, const glm::vec3& value) const;
		void setFloat(const char* name, const float value) const;
	private:
		bool m_isCompiled = false;
		unsigned int m_id = 0;
	};
}
