#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Objects/Material.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace My_Ezz
{
	BaseMesh::BaseMesh()
		: m_pVAO(std::make_unique<VertexArray>()),
		m_dScale(1.0f),
		m_mProjectionMatrix(1.0f),
		m_mViewMatrix(1.0f),
		m_vPosition(1.0f),
		m_vRotation(0.0f),
		m_pMaterial(nullptr)
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
		m_pVAO->addVertexBuffer(*m_pPositions.get());
		m_pVAO->setIndexBuffer(*m_pIndexes.get());
	}

	void BaseMesh::LoadMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes)
	{
		m_pVAO->addVertexBuffer(*m_pPositions.get());
		m_pVAO->setIndexBuffer(*m_pIndexes.get());
	}

	void BaseMesh::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		if (m_pMaterial && m_pMaterial->IsUsingDiffuseMap())
		{
			m_pMaterial->GetDiffuseMap()->bind(0);
			pShaderProgram->setUniformValue("u_diffuseMap", 0);
		}

		if (m_pMaterial && m_pMaterial->IsUsingNormalMap())
		{
			m_pMaterial->GetNormalMap()->bind(1);
			pShaderProgram->setUniformValue("u_normalMap", 1);
		}

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_vPosition);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_vRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_vRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(m_vRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelMatrix = glm::mat4_cast(glm::qua(glm::radians(m_vRotation)))*modelMatrix;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(m_dScale));
		modelMatrix = glm::mat4(1.0f) * modelMatrix;
		pShaderProgram->setUniformValue("u_modelMatrix", modelMatrix);

		if (m_pMaterial)
		{
			pShaderProgram->setUniformValue("u_materialProperty.ambientColor", m_pMaterial->GetAmbientColor());
			pShaderProgram->setUniformValue("u_materialProperty.diffuseColor", m_pMaterial->GetDiffuseColor());
			pShaderProgram->setUniformValue("u_materialProperty.specularColor", m_pMaterial->GetSpeculatColor());
			pShaderProgram->setUniformValue("u_materialProperty.shines", m_pMaterial->GetShines());
			pShaderProgram->setUniformValue("u_isUsingDiffuseMap", m_pMaterial->IsUsingDiffuseMap());
			pShaderProgram->setUniformValue("u_isUsingNormalMap", m_pMaterial->IsUsingNormalMap());
		}

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

	void BaseMesh::SetMaterial(std::shared_ptr<Material> pMaterial)
	{
		m_pMaterial = pMaterial;
	}

}
