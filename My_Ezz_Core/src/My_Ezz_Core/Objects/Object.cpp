#include "My_Ezz_Core/Objects/Object.hpp"

#include "My_Ezz_Core/Objects/Image/Texture2D.hpp"
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"
#include "My_Ezz_Core/Objects/MaterialLib.hpp"


namespace My_Ezz
{

	Object::Object()
	{
		m_pMtlLib = std::make_shared<MaterialLibrary>();
	}

	void Object::Rotate(const glm::vec3& vRotation)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->Rotate(vRotation);
	}

	void Object::Translate(const glm::vec3& vTraslate)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->Translate(vTraslate);
	}

	void Object::Scale(const double& dScale)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->Scale(dScale);
	}

	void Object::SetPosition(const glm::vec3& vPosition)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->SetPosition(vPosition);
	}

	void Object::SetRotation(const glm::vec3& vRotation)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->SetRotation(vRotation);
	}

	void Object::SetScale(const double& dScale)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->SetScale(dScale);
	}

	void Object::SetTexture(std::shared_ptr<Texture2D> pTexture)
	{
		m_pTexture = pTexture;
	}

	void Object::AddMesh(std::shared_ptr<BaseMesh> pMesh)
	{
		if (!pMesh)
			return;

		auto It = std::find_if(m_vMeshes.begin(), m_vMeshes.end(),
			[pMesh](const std::shared_ptr<BaseMesh> pTempMesh)
			{
				return pMesh == pTempMesh;
			});

		if (It == m_vMeshes.end())
			m_vMeshes.push_back(pMesh);
	}

	void Object::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		if (m_pTexture)
			m_pTexture->bind(0);
		for (auto pMesh : m_vMeshes)
			pMesh->Draw(pShaderProgram);
	}

	void Object::SetShaderMatrix(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->SetShaderMatrixs(mProjectionMatrix, mViewMatrix);
	}

	std::shared_ptr<My_Ezz::MaterialLibrary> Object::GetMtlLib() const
	{
		return m_pMtlLib;
	}

}

