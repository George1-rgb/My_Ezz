#pragma once
#include <memory>
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"

namespace My_Ezz
{

	class LightMesh: public BaseMesh
	{
	public:
		LightMesh();
		LightMesh(std::shared_ptr<VertexBuffer> m_pPositions, std::shared_ptr<IndexBuffer> m_pIndexes);
		virtual  ~LightMesh() {}

		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram) override;
		void SetColor(const glm::vec3& vColor);
	protected:
		glm::vec3 m_vLightSourcePosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_vLightSourceColor = { 1.0f, 1.0f, 1.0f };
		double m_dAmbientFactor = 0.1f;
		double m_dDiffuseFactor = 1.f;
		double m_dSpecularFactor = 0.5f;
		double m_dShininess = 32.f;
	};
}