#pragma once
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <string>

namespace My_Ezz
{
	class ShaderProgram;
	class VertexBuffer;
	class VertexArray;
	class IndexBuffer;
	class Texture2D;


	class IDrawing
	{
	public:
		IDrawing() {}
		virtual ~IDrawing() {}
		virtual void LoadMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes) = 0;
		virtual void Draw(std::shared_ptr<ShaderProgram> pShaderProgram) = 0;
		virtual void SetShaderMatrixs(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix) = 0;
	};

	class ITransforming
	{
	public:
		ITransforming() {}
		virtual ~ITransforming() {}
		virtual void Rotate(const glm::vec3& vRotation) = 0;
		virtual void Translate(const glm::vec3& vTraslate) = 0;
		virtual void Scale(const double& dScale) = 0;
		virtual void SetPosition(const glm::vec3& vPosition) = 0;
		virtual void SetRotation(const glm::vec3& vRotation) = 0;
		virtual void SetScale(const double& dScale) = 0;
	};

	class ISerializable
	{
	public:
		ISerializable() {}
		virtual ~ISerializable() {}
	};
}
