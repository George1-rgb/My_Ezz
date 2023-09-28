#include "My_Ezz_Core/Objects/Mesh/LightMesh.hpp"


#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"

namespace My_Ezz
{
	

	LightMesh::LightMesh()
		: BaseMesh()
	{

	}

	LightMesh::LightMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes)
		: BaseMesh(m_pPositions, m_pIndexes)
	{

	}

	void LightMesh::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		glm::mat4 mModelMatrix(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, m_vPosition);
		mModelMatrix = glm::mat4_cast(glm::qua(glm::radians(m_vRotation))) * mModelMatrix;
		mModelMatrix = glm::scale(mModelMatrix, glm::vec3(m_dScale));

		glm::mat4 mvpMatrix = m_mProjectionMatrix * m_mViewMatrix * mModelMatrix;
		pShaderProgram->setMatrix4("mvpMatrix", mvpMatrix);
		pShaderProgram->setVec3("light_color", m_vLightSourceColor);

		Renderer_OpenGL::draw(*m_pVAO);
	}

	void LightMesh::SetColor(const glm::vec3& vColor)
	{
		m_vLightSourceColor = vColor;
	}

}
