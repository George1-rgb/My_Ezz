#pragma once
#include <memory>
#include "../Interfaces.hpp"

namespace My_Ezz
{
	class BaseMesh;
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

		void SetMesh(std::shared_ptr<BaseMesh> pMesh);
		std::shared_ptr<BaseMesh> GetMesh() const;
		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram);

	protected:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<BaseMesh> m_pMesh;

		static int m_nCounter;
	};
}
