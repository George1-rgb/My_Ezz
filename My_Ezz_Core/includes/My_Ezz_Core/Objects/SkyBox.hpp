#pragma once
#include <memory>
#include "My_Ezz_Core/Objects/Object.hpp"


namespace My_Ezz
{
	class Texture2D;
	class SkyBox :public Object
	{
	public:
		SkyBox(const double& dWidth, const std::shared_ptr<Texture2D> pTexture);
		virtual ~SkyBox() {}

		//ITransforming
		virtual void Rotate(const glm::vec3& vRotation) override;
		virtual void Translate(const glm::vec3& vTraslate) override;
		virtual void Scale(const double& dScale) override;
		virtual void SetPosition(const glm::vec3& vPosition) override;
		virtual void SetRotation(const glm::vec3& vRotation) override;
		virtual void SetScale(const double& dScale) override;
	};
}