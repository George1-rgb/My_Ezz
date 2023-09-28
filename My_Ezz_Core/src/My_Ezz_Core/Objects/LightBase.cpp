#include "My_Ezz_Core/Objects/LightBase.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/Texture2D.hpp"
#include "My_Ezz_Core/Objects/Mesh/LightMesh.hpp"

namespace My_Ezz
{
	LightBase::LightBase()
		: Object()
	{

	}

	void LightBase::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		std::shared_ptr<LightMesh> pLightMesh = std::dynamic_pointer_cast<LightMesh>(m_pMesh);
		pLightMesh->Draw(pShaderProgram);
	}

	void LightBase::SetColor(const glm::vec3& vColor)
	{
		std::shared_ptr<LightMesh> pLightMesh = std::dynamic_pointer_cast<LightMesh>(m_pMesh);
		pLightMesh->SetColor(vColor);
	}
}

