#pragma once
#include "Object.hpp"

namespace My_Ezz
{
	class LightMesh;
	class LightBase : public Object
	{
	public:
		LightBase();
		virtual ~LightBase() {}

		void Draw(std::shared_ptr<ShaderProgram> pShaderProgram);
		
		void SetColor(const glm::vec3& vColor);
	};
}