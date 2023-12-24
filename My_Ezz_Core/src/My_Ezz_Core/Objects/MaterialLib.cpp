#include "My_Ezz_Core/Objects/MaterialLib.hpp"
#include "My_Ezz_Core/Objects/Material.hpp"

namespace My_Ezz
{
MaterialLibrary::MaterialLibrary()
{

}

void MaterialLibrary::AddMaterial(std::shared_ptr<Material> pMaterial)
{
	if (!pMaterial)
		return;
	if (GetMaterial(pMaterial->GetMtlName()))
		return;
	
	m_vMaterials.push_back(pMaterial);
}

std::shared_ptr<Material> MaterialLibrary::GetMaterial(const unsigned int& nIndex) const
{
	if (nIndex >= m_vMaterials.size())
		return nullptr;
	
	return m_vMaterials[nIndex];
}

const size_t MaterialLibrary::GetCountOfMaterials() const
{
	return m_vMaterials.size();
}

std::shared_ptr<Material> MaterialLibrary::GetMaterial(const std::string& strMtlName) const
{
	auto It = std::find_if(m_vMaterials.begin(), m_vMaterials.end(),
		[strMtlName](const std::shared_ptr<Material> pTempMaterial)
		{
			return pTempMaterial->GetMtlName() == strMtlName;
		});
	if (It != m_vMaterials.end())
		return *It;

	return nullptr;
}
}
