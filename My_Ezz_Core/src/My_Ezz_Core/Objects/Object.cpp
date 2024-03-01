#include "My_Ezz_Core/Objects/Object.hpp"

#include "My_Ezz_Core/Objects/Image/Texture2D.hpp"
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"
#include "My_Ezz_Core/Objects/MaterialLib.hpp"


namespace My_Ezz
{

	int Object::m_nIDGen = 0;
	Object::Object(const std::string& strName) 
		: m_strName(strName)
	{
		m_pMtlLib = std::make_shared<MaterialLibrary>();
		m_nID = ++m_nIDGen;
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

	void Object::Draw(std::shared_ptr<ShaderProgram> pShaderProgram, bool bSelected/* = false*/)
	{
		for (auto pMesh : m_vMeshes)
			pMesh->Draw(pShaderProgram, bSelected);
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

	bool Object::Load(const rapidjson::Value& obj)
	{
		return true;
	}

	bool Object::Save(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
	{
		writer->StartObject();

		writer->String("object_name"); // create Id property
		writer->String(m_strName.c_str());     // write the Id value from the object instance

		writer->String("meshes");
		writer->StartArray();
		{
			for (auto& pMesh : m_vMeshes)
			{
				writer->String(pMesh->GetName().c_str());
			}
		}
		writer->EndArray();

		writer->EndObject();

		return true;
	}

	bool Object::SaveRefs(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
	{
		return true;
	}

}

