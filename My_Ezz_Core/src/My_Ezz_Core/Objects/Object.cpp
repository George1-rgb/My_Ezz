#include "My_Ezz_Core/Objects/Object.hpp"

#include "My_Ezz_Core/Rendering/OpenGL/Texture2D.hpp"
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"



namespace My_Ezz
{
	int Object::m_nCounter;

	Object::Object()
	{
		if (m_nCounter < 0) m_nCounter = 0;

		m_nCounter++;
	}

	void Object::Rotate(const glm::vec3& vRotation)
	{
		m_pMesh->Rotate(vRotation);
	}

	void Object::Translate(const glm::vec3& vTraslate)
	{
		m_pMesh->Translate(vTraslate);
	}

	void Object::Scale(const double& dScale)
	{
		m_pMesh->Scale(dScale);
	}

	void Object::SetPosition(const glm::vec3& vPosition)
	{
		m_pMesh->SetPosition(vPosition);
	}

	void Object::SetRotation(const glm::vec3& vRotation)
	{
		m_pMesh->SetRotation(vRotation);
	}

	void Object::SetScale(const double& dScale)
	{
		m_pMesh->SetScale(dScale);
	}

	void Object::SetTexture(std::shared_ptr<Texture2D> pTexture)
	{
		m_pTexture = pTexture;
		//m_pTexture->bind(m_nCounter);
	}

	void Object::SetMesh(std::shared_ptr<BaseMesh> pMesh)
	{
		m_pMesh = pMesh;
	}

	std::shared_ptr<My_Ezz::BaseMesh> Object::GetMesh() const
	{
		return m_pMesh;
	}

	void Object::Draw(std::shared_ptr<ShaderProgram> pShaderProgram)
	{
		m_pMesh->Draw(pShaderProgram);
	}

}

