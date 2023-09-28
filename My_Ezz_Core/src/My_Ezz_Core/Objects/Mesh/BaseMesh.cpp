#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace My_Ezz
{
	BaseMesh::BaseMesh()
		: m_pVAO(std::make_unique<VertexArray>()),
		m_dScale(1.0f),
		m_mProjectionMatrix(1.0f),
		m_mViewMatrix(1.0f),
		m_vPosition(1.0f),
		m_vRotation(0.0f)
	{

	}

	BaseMesh::BaseMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes)
		: m_pVAO(std::make_unique<VertexArray>()),
		m_dScale(1.0f),
		m_mProjectionMatrix(1.0f),
		m_mViewMatrix(1.0f),
		m_vPosition(1.0f),
		m_vRotation(0.0f)
	{
		m_pVAO->addVertexBuffer(*m_pPositions);
		m_pVAO->setIndexBuffer(*m_pIndexes);
	}

	void BaseMesh::LoadMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes)
	{
		m_pVAO->addVertexBuffer(*m_pPositions);
		m_pVAO->setIndexBuffer(*m_pIndexes);
	}

	void BaseMesh::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_vPosition);
		modelMatrix = glm::mat4_cast(glm::qua(glm::radians(m_vRotation))) * modelMatrix;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(m_dScale));

		const glm::mat4 modelViewMatrix = m_mViewMatrix * modelMatrix;
		pShaderProgram->setMatrix4("modelViewMatrix", modelViewMatrix);

		const glm::mat4 mvpMatrix = m_mProjectionMatrix * modelViewMatrix;
		pShaderProgram->setMatrix4("mvpMatrix", mvpMatrix);

		const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
		pShaderProgram->setMatrix3("normalMatrix", normalMatrix);

		Renderer_OpenGL::draw(*m_pVAO);
	}

	void BaseMesh::SetShaderMatrixs(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix)
	{
		m_mProjectionMatrix = mProjectionMatrix;
		m_mViewMatrix = mViewMatrix;
	}

	void BaseMesh::Rotate(const glm::vec3& vRotation)
	{
		m_vRotation += vRotation;
	}

	void BaseMesh::Translate(const glm::vec3& vTraslate)
	{
		m_vPosition += vTraslate;
	}

	void BaseMesh::Scale(const double& dScale)
	{
		m_dScale *= dScale;
	}

	void BaseMesh::SetPosition(const glm::vec3& vPosition)
	{
		m_vPosition = vPosition;
	}

	void BaseMesh::SetRotation(const glm::vec3& vRotation)
	{
		m_vRotation = vRotation;
	}

	void BaseMesh::SetScale(const double& dScale)
	{
		m_dScale = dScale;
	}

}
