#include "My_Ezz_Core/Objects/LightBase.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/Renderer_OpenGL.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/Texture2D.hpp"

namespace My_Ezz
{
	LightBase::LightBase()
		: Object()
	{

	}

	LightBase::LightBase(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes)
		: Object(m_pPositions, m_pIndexes)
	{

	}

	void LightBase::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		glm::mat4 mModelMatrix(1.0f);
		mModelMatrix = glm::translate(mModelMatrix, m_vTraslation);
		glm::mat4 mvpMatrix = m_mProjectionMatrix * m_mViewMatrix * mModelMatrix;
		pShaderProgram->setMatrix4("mvpMatrix", mvpMatrix);
		pShaderProgram->setVec3("light_color", m_vLightSourceColor);

		Renderer_OpenGL::draw(*m_pVAO);
	}

	void LightBase::SetColor(const glm::vec3& vColor)
	{
		m_vLightSourceColor = vColor;
	}

}

