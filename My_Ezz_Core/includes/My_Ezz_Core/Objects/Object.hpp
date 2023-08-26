#pragma once
#include <memory>
#include "../Interfaces.hpp"

namespace My_Ezz
{

	class Object : public IDrawing, public ITransforming
	{
	public:
		Object();
		Object(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes);

		virtual ~Object() {}

		//IDrawing
		void LoadObject(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes) override;
		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram) override;
		void SetShaderMatrixs(const glm::mat4& mProjectionMatrix, const glm::mat4& mViewMatrix) override;
		//ITransforming
		virtual void Rotate(const glm::quat& qQuat) override;
		virtual void Translate(const glm::vec3& vTraslate) override;
		virtual void Scale(const double& dScale) override;

	protected:
		std::shared_ptr<Texture2D> m_pTexture;

		std::shared_ptr<VertexArray> m_pVAO;
		glm::mat4 m_mProjectionMatrix;
		glm::mat4 m_mViewMatrix;

		glm::vec3 m_vTraslation;
		double m_dScale;
		glm::quat m_qQuaternion;
	};
}
