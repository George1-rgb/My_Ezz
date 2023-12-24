#include "My_Ezz_Core/Objects/LightBase.hpp"
#include "My_Ezz_Core/Objects/Image/Texture2D.hpp"
#include "My_Ezz_Core/Objects/Mesh/LightMesh.hpp"

namespace My_Ezz
{
	LightBase::LightBase()
		: Object()
	{

	}

	void LightBase::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		for (auto pMesh : m_vMeshes)
		{
			std::shared_ptr<LightMesh> pLightMesh = std::dynamic_pointer_cast<LightMesh>(pMesh);
			if (pLightMesh)
				pLightMesh->Draw(pShaderProgram);
		}
			
	}

	void LightBase::SetColor(const glm::vec3& vColor)
	{
		for (auto pMesh : m_vMeshes)
		{
			std::shared_ptr<LightMesh> pLightMesh = std::dynamic_pointer_cast<LightMesh>(pMesh);
			if (pLightMesh)
				pLightMesh->SetColor(vColor);
		}
	}
}

