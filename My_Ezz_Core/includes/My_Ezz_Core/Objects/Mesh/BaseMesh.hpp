#pragma once
#include <memory>
#include "../../Interfaces.hpp"

namespace My_Ezz
{

	class BaseMesh: public IDrawing, public ITransforming
	{
	public:
		BaseMesh();
		BaseMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes);
		virtual  ~BaseMesh() {}


		//IDrawing
		void LoadMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes) override;
		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram) override;
		void SetShaderMatrixs(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix) override;

		//ITransforming
		virtual void Rotate(const glm::vec3& vRotation) override;
		virtual void Translate(const glm::vec3& vTraslate) override;
		virtual void Scale(const double& dScale) override;
		virtual void SetPosition(const glm::vec3& vPosition) override;
		virtual void SetRotation(const glm::vec3& vRotation) override;
		virtual void SetScale(const double& dScale) override;

	protected:
		std::shared_ptr<VertexArray> m_pVAO;
		glm::mat4 m_mProjectionMatrix;
		glm::mat4 m_mViewMatrix;

		glm::vec3 m_vPosition;
		double m_dScale;
		glm::vec3 m_vRotation;
	};
}