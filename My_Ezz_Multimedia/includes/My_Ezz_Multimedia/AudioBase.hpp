#pragma once
#include <iostream>

namespace My_Ezz
{
	class AudioBase
	{
	public:
		AudioBase(char* soundData, const std::uint8_t& channels, const std::int32_t& sampleRate, const std::uint8_t& bitsPerSample, const std::int32_t& size);
		virtual ~AudioBase();
		AudioBase(const AudioBase&) = delete;
		AudioBase& operator=(const AudioBase&) = delete;
		virtual void PlaySound();
		virtual void StopSound();
		virtual void SetLooping(bool bLopping);
		virtual void SetPitch(float fPitch);
		virtual void SetGain(float fGain);
		virtual void SetPosition(float fX, float fY, float fZ);
		virtual void SetVelocity(float fX, float fY, float fZ);

	private:
		std::uint8_t m_channels;
		std::int32_t m_sampleRate;
		std::uint8_t m_bitsPerSample;
		std::int32_t m_size;
		char* m_soundData;
		std::uint32_t m_buffer;
		std::uint32_t m_source;
	};
}
