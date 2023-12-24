#include "My_Ezz_Core/Objects/Material.hpp"
#include "My_Ezz_Core/ResourceManager.hpp"

namespace My_Ezz
{
	Material::Material()
		:m_bNormalMap(false), 
		m_bDiffuseMap(false),
		m_strMtlName(""),
		m_vDiffuseColor(glm::vec3(0.f)),
		m_vAmbientColor(glm::vec3(0.f)),
		m_vSpecularColor(glm::vec3(0.f)),
		m_dShines(1.0),
		m_pDiffuseMap(nullptr),
		m_pNormalMap(nullptr)
	{

	}

	void Material::SetName(const std::string& strMtlName)
	{
		m_strMtlName = strMtlName;
	}

	const std::string& Material::GetMtlName() const
	{
		return m_strMtlName;
	}


	void Material::SetDiffuseColor(const glm::vec3& vDiffuseColor)
	{
		m_vDiffuseColor = vDiffuseColor;
	}

	const glm::vec3& Material::GetDiffuseColor() const
	{
		return m_vDiffuseColor;
	}

	void Material::SetAmbientColor(const glm::vec3& vAmbientColor)
	{
		m_vAmbientColor = vAmbientColor;
	}

	const glm::vec3& Material::GetAmbientColor() const
	{
		return m_vAmbientColor;
	}

	void Material::SetSpecularColor(const glm::vec3& vSpecularColor)
	{
		m_vSpecularColor = vSpecularColor;
	}

	const glm::vec3& Material::GetSpeculatColor() const
	{
		return m_vSpecularColor;
	}

	void Material::SetShines(const double& dShines)
	{
		m_dShines = dShines;
	}

	const float& Material::GetShines() const
	{
		return m_dShines;
	}

	void Material::SetDiffuseMap(const std::shared_ptr<Texture2D>& pTexture)
	{
		m_pDiffuseMap = pTexture;
		m_bDiffuseMap = true;
	}

	const std::shared_ptr<Texture2D>& Material::GetDiffuseMap() const
	{
		return m_pDiffuseMap;
	}

	bool Material::IsUsingDiffuseMap() const
	{
		return m_bDiffuseMap;
	}

	void Material::SetNormalMap(const std::shared_ptr<Texture2D>& pTexture)
	{
		m_pNormalMap = pTexture;
		m_bNormalMap = true;
	}

	const std::shared_ptr<Texture2D>& Material::GetNormalMap() const
	{
		return m_pNormalMap;
	}

	bool Material::IsUsingNormalMap() const
	{
		return m_bNormalMap;
	}
}

