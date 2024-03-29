#pragma once
#include <memory>
#include <vector>
#include "../Interfaces.hpp"

namespace My_Ezz
{
	class BaseMesh;
	class MaterialLibrary;
	class Object : public ISerializable
	{
	public:
		Object(const std::string& strName = "none");

		virtual ~Object() {}

		virtual void Rotate(const glm::vec3& vRotation);
		virtual void Translate(const glm::vec3& vTraslate);
		virtual void Scale(const double& dScale);
		virtual void SetPosition(const glm::vec3& vPosition);
		virtual void SetRotation(const glm::vec3& vRotation);
		virtual void SetScale(const double& dScale);

		void AddMesh(std::shared_ptr<BaseMesh> pMesh);
		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram, bool bSelected = false);
		void SetShaderMatrix(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix);
		std::shared_ptr<MaterialLibrary> GetMtlLib() const;

		//ISerializable
		virtual bool Load(const rapidjson::Value& obj) override;
		virtual bool Save(rapidjson::Writer<rapidjson::StringBuffer>* writer) const override;
		virtual bool SaveRefs(rapidjson::Writer<rapidjson::StringBuffer>* writer) const override;

		int GetID() { return m_nID; }
		std::string GetName() { return m_strName; }
	protected:
		std::shared_ptr<MaterialLibrary> m_pMtlLib;
		std::vector<std::shared_ptr<BaseMesh>> m_vMeshes;
		std::string m_strName;
		static int m_nIDGen;
		int m_nID;
	};
}
