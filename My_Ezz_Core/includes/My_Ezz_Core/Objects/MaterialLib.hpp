#pragma once
#include <memory>
#include <vector>
#include <string>

namespace My_Ezz
{
class Material;

class MaterialLibrary
{
public:
	MaterialLibrary();
	void AddMaterial(std::shared_ptr<Material> pMaterial);

	std::shared_ptr<Material> GetMaterial(const unsigned int& nIndex) const;
	std::shared_ptr<Material> GetMaterial(const std::string& strMtlName) const;

	const size_t GetCountOfMaterials() const;

private:
	std::vector<std::shared_ptr<Material>> m_vMaterials;
};
}