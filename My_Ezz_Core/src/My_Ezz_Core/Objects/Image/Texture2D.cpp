#include "My_Ezz_Core/Objects/Image/Texture2D.hpp"

#include <algorithm>
#include <cmath>
#include <glad/glad.h>

namespace My_Ezz
{
	Texture2D::Texture2D(const unsigned char* cData, const unsigned int nWidth, const unsigned int nHeight, TextureType textureType)
		: m_nWidth(nWidth), m_nHeight(nHeight)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &m_nID);
		const GLsizei mipLevels = static_cast<GLsizei>(log2(std::max(m_nWidth, m_nHeight))) + 1;
		glTextureStorage2D(m_nID, mipLevels, GL_RGB8, m_nWidth, m_nHeight);
		glTextureSubImage2D(m_nID, 0, 0, 0, m_nWidth, m_nHeight, textureType == TextureType::kDiffuse ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, cData);
		glTextureParameteri(m_nID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_nID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(m_nID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTextureParameteri(m_nID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glGenerateTextureMipmap(m_nID);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_nID);
	}

	Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept
	{
		glDeleteTextures(1, &m_nID);
		m_nID = texture.m_nID;
		m_nWidth = texture.m_nWidth;
		m_nHeight = texture.m_nHeight;
		texture.m_nID = 0;
		return *this;
	}

	Texture2D::Texture2D(Texture2D&& texture) noexcept
	{
		m_nID = texture.m_nID;
		m_nWidth = texture.m_nWidth;
		m_nHeight = texture.m_nHeight;
		texture.m_nID = 0;
	}

	void Texture2D::bind(const unsigned int nUnit) const
	{
		glBindTextureUnit(nUnit, m_nID);
	}

	void Texture2D::unbind(const unsigned int nUnit) const
	{
		glBindTextureUnit(nUnit, 0);
	}

}