#pragma once
#include <string>
#include "My_Ezz_Core/Objects/Image/Texture2D.hpp"
#include <memory>
#include <glm/vec3.hpp>

namespace My_Ezz
{
	class Material
	{
	public:
		Material();
		void SetName(const std::string& strMtlName);
		const std::string& GetMtlName() const;

		void SetDiffuseColor(const glm::vec3& vDiffuseColor);
		const glm::vec3& GetDiffuseColor() const;

		void SetAmbientColor(const glm::vec3& vAmbientColor);
		const glm::vec3& GetAmbientColor() const;

		void SetSpecularColor(const glm::vec3& vSpecularColor);
		const glm::vec3& GetSpeculatColor() const;

		void SetShines(const double& dShines);
		const float& GetShines() const;

		void SetDiffuseMap(const std::shared_ptr<Texture2D>& pTexture);
		const std::shared_ptr<Texture2D>& GetDiffuseMap() const;
		bool IsUsingDiffuseMap() const;

		void SetNormalMap(const std::shared_ptr<Texture2D>& pTexture);
		const std::shared_ptr<Texture2D>& GetNormalMap() const;
		bool IsUsingNormalMap() const;

	private:
		std::string m_strMtlName;
		glm::vec3 m_vDiffuseColor;
		glm::vec3 m_vAmbientColor;
		glm::vec3 m_vSpecularColor;
		double m_dShines;
		std::shared_ptr<Texture2D> m_pDiffuseMap;
		std::shared_ptr<Texture2D> m_pNormalMap;
		bool m_bDiffuseMap;
		bool m_bNormalMap;
	};
}