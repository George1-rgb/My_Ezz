#pragma once

namespace My_Ezz
{
	class Texture2D
	{
	public:
		Texture2D(const unsigned char* cData, const unsigned int nWidth, const unsigned int nHeight);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture) noexcept;
		Texture2D(Texture2D&& texture) noexcept;

		void bind(const unsigned int nUnit) const;
		void unbind(const unsigned int nUnit) const;

	private:
		unsigned int m_nID = 0;
		unsigned int m_nWidth = 0;
		unsigned int m_nHeight = 0;
	};
}
