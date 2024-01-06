#include "My_Ezz_Core/Objects/SkyBox.hpp"
#include <GLFW/glfw3.h>
#include "My_Ezz_Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/VertexArray.hpp"
#include "My_Ezz_Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "My_Ezz_Core/Objects/Mesh/BaseMesh.hpp"
#include "My_Ezz_Core/Objects/Material.hpp"

namespace My_Ezz
{
	SkyBox::SkyBox(const double& dWidth, const std::shared_ptr<Texture2D> pTexture)
	{
		float w = static_cast<float>(dWidth / 2.0);
		std::vector<GLfloat> vertBuffer;

		//-y Слева
		vertBuffer.insert(vertBuffer.end(), { -w, -w, -w, 1.0f, 2.0f / 3.0f, 0.0f, 0.0f, -1.0f }); //1.0f, 2.0f / 3.0f
		vertBuffer.insert(vertBuffer.end(), { -w, -w, w, 1.0f, 1.0 / 3.0f,0.0f, 0.0f, -1.0f }); //1.0f, 1.0 / 3.0f
		vertBuffer.insert(vertBuffer.end(), { w, -w, -w, 3.0f / 4.0f, 2.0f / 3.0f, 0.0f, 0.0f, -1.0f }); //3.0f / 4.0f, 2.0f / 3.0f
		vertBuffer.insert(vertBuffer.end(), { w, -w, w,3.0f / 4.0f, 1.0f / 3.0f, 0.0f, 0.0f, -1.0f });	//3.0f / 4.0f, 1.0f / 3.0f	

		//x Спереди
		vertBuffer.insert(vertBuffer.end(), { w, -w, -w, 3.0f / 4.0f, 2.0f / 3.0f, -1.0f, 0.0f, 0.0f }); //3.0f / 4.0f, 2.0f / 3.0f
		vertBuffer.insert(vertBuffer.end(), {w, -w, w, 3.0f / 4.0f, 1.0f / 3.0f, -1.0f, 0.0f, 0.0f}); //3.0f / 4.0f, 1.0f / 3.0f
		vertBuffer.insert(vertBuffer.end(), { w, w, -w, 2.0f / 4.0f, 2.0f / 3.0f,-1.0f, 0.0f, 0.0f }); //2.0f / 4.0f, 2.0f / 3.0f
		vertBuffer.insert(vertBuffer.end(), { w, w, w, 2.0f / 4.0f, 1.0f / 3.0f, -1.0f, 0.0f, 0.0f }); //2.0f / 4.0f, 1.0f / 3.0f
	
		//z Над нами
		vertBuffer.insert(vertBuffer.end(), { w, -w, -w,2.0f / 4.0f, 1.0f, 0.0f, -1.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { w, w, -w, 2.0f / 4.0f, 2.0f / 3.0f, 0.0f, -1.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, -w, -w, 1.0f / 4.0f, 1.0f, 0.0f, -1.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, w, -w, 1.0f / 4.0f, 2.0f / 3.0f, 0.0f, -1.0f, 0.0f });
	
		//y Справа
		vertBuffer.insert(vertBuffer.end(), { w, w, -w, 2.0f / 4.0f, 2.0f / 3.0f, 0.0f, 0.0f, 1.0f });
		vertBuffer.insert(vertBuffer.end(), { w, w, w, 2.0f / 4.0f, 1.0f / 3.0f, 0.0f, 0.0f, 1.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, w, -w, 1.0f / 4.0f, 2.0f / 3.0f, 0.0f, 0.0f, 1.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, w, w, 1.0f / 4.0f, 1.0f / 3.0f, 0.0f, 0.0f, 1.0f });
	
		//-x Сзади
		vertBuffer.insert(vertBuffer.end(), { -w, w, -w, 1.0f / 4.0f, 2.0f / 3.0f, 1.0f, 0.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, w, w, 1.0f / 4.0f, 1.0f / 3.0f, 1.0f, 0.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, -w, -w, 0.0f, 2.0f / 3.0f, 1.0f, 0.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, -w, w, 0.0f, 1.0f / 3.0f, 1.0f, 0.0f, 0.0f });
	
		//-z Под нами
		vertBuffer.insert(vertBuffer.end(), { w, w, w, 2.0f / 4.0f, 1.0f / 3.0f, 0.0f, 1.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { w, -w, w, 2.0f / 4.0f, 0.0f, 0.0f, 1.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, w, w, 1.0f / 4.0f, 1.0f / 3.0f, 0.0f, 1.0f, 0.0f });
		vertBuffer.insert(vertBuffer.end(), { -w, -w, w, 1.0f / 4.0f, 0.0f, 0.0f, 1.0f, 0.0f });

		std::vector<GLint> indexBuffer;
		for (int i = 0; i <= 24; i += 4)
		{
			indexBuffer.push_back(i + 0);
			indexBuffer.push_back(i + 2);
			indexBuffer.push_back(i + 1);

			indexBuffer.push_back(i + 2);
			indexBuffer.push_back(i + 3);
			indexBuffer.push_back(i + 1);
		}

		BufferLayout bufferLayout_2vec3_vec2
		{
			My_Ezz::ShaderDataType::Float3,
			My_Ezz::ShaderDataType::Float2,
			My_Ezz::ShaderDataType::Float3
		};
		std::shared_ptr<My_Ezz::BaseMesh> pMesh = std::make_shared<My_Ezz::BaseMesh>();
		std::shared_ptr<My_Ezz::Material> pNewMaterial = std::make_shared<My_Ezz::Material>();
		pNewMaterial->SetName("SkyBoxMTL");
		pNewMaterial->SetDiffuseMap(pTexture);
		pNewMaterial->SetShines(96.0f);
		pNewMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0, 1.0f));
		pNewMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0, 1.0f));
		pNewMaterial->SetAmbientColor(glm::vec3(1.0f, 1.0, 1.0f));
		std::shared_ptr<My_Ezz::VertexBuffer> vertexes;
		std::shared_ptr<My_Ezz::IndexBuffer> indexes;
		std::vector<GLfloat> tmpVertBuff{ std::make_move_iterator(std::begin(vertBuffer)), std::make_move_iterator(std::end(vertBuffer)) };
		std::vector<GLint> tmpIndexBuff{ std::make_move_iterator(std::begin(indexBuffer)), std::make_move_iterator(std::end(indexBuffer)) };
		vertexes = std::make_shared<My_Ezz::VertexBuffer>(tmpVertBuff.data(), tmpVertBuff.size() * sizeof(GLfloat), bufferLayout_2vec3_vec2);
		indexes = std::make_shared<My_Ezz::IndexBuffer>(tmpIndexBuff.data(), tmpIndexBuff.size());
		pMesh->LoadMesh(vertexes, indexes);
		pMesh->SetMaterial(pNewMaterial);
		AddMesh(pMesh);

}

void SkyBox::Rotate(const glm::vec3& vRotation)
{
	(void)vRotation;
}

void SkyBox::Translate(const glm::vec3& vTraslate)
{
	(void)vTraslate;
}

void SkyBox::Scale(const double& dScale)
{
	(void)dScale;
}

void SkyBox::SetPosition(const glm::vec3& vPosition)
{
	(void)vPosition;
}

void SkyBox::SetRotation(const glm::vec3& vRotation)
{
	(void)vRotation;
}

void SkyBox::SetScale(const double& dScale)
{
	(void)dScale;
}

}

