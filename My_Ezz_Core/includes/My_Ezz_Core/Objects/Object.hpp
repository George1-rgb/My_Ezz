#pragma once
#include <memory>
#include <vector>
#include "../Interfaces.hpp"

namespace My_Ezz
{
	class BaseMesh;
	class MaterialLibrary;
	class Object
	{
	public:
		Object();

		virtual ~Object() {}

		virtual void Rotate(const glm::vec3& vRotation);
		virtual void Translate(const glm::vec3& vTraslate);
		virtual void Scale(const double& dScale);
		virtual void SetPosition(const glm::vec3& vPosition);
		virtual void SetRotation(const glm::vec3& vRotation);
		virtual void SetScale(const double& dScale);

		void SetTexture(std::shared_ptr<Texture2D> pTexture);

		void AddMesh(std::shared_ptr<BaseMesh> pMesh);
		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram);
		void SetShaderMatrix(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix);
		std::shared_ptr<MaterialLibrary> GetMtlLib() const;
	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<MaterialLibrary> m_pMtlLib;
		std::vector<std::shared_ptr<BaseMesh>> m_vMeshes;


	};
}
