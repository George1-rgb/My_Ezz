#include "My_Ezz_Multimedia/AudioBase.hpp"
#include "My_Ezz_Logger/Log.hpp"
#include "al.h"
#include <alc.h>



namespace My_Ezz
{
	AudioBase::AudioBase(char* soundData, const std::uint8_t& channels, const std::int32_t& sampleRate, const std::uint8_t& bitsPerSample, const std::int32_t& size)
		: m_soundData(soundData), m_channels(channels), m_sampleRate(sampleRate), m_bitsPerSample(bitsPerSample), m_size(size), m_source(0)
	{
		alGenBuffers(1, &m_buffer);

		ALenum format;
		if (channels == 1 && bitsPerSample == 8)
			format = AL_FORMAT_MONO8;
		else if (channels == 1 && bitsPerSample == 16)
			format = AL_FORMAT_MONO16;
		else if (channels == 2 && bitsPerSample == 8)
			format = AL_FORMAT_STEREO8;
		else if (channels == 2 && bitsPerSample == 16)
			format = AL_FORMAT_STEREO16;
		else
		{
			LOG_CRITICAL("ERROR: unrecognised wave format: {0} channels, {1} bps", channels, bitsPerSample);
			return;
		}

		alBufferData(m_buffer, format, soundData, size, sampleRate);

		alGenSources(1, &m_source);
		alSourcef(m_source, AL_PITCH, 1);
		alSourcef(m_source, AL_GAIN, 1.0f);
		alSource3f(m_source, AL_POSITION, 0, 0, 0);
		alSource3f(m_source, AL_VELOCITY, 0, 0, 0);
		alSourcei(m_source, AL_LOOPING, AL_FALSE);
		alSourcei(m_source, AL_SOURCE_RELATIVE, AL_FALSE);
		alSourcei(m_source, AL_BUFFER, m_buffer);
	}

	void AudioBase::PlaySound()
	{
		alSourcePlay(m_source);
	}

	void AudioBase::StopSound()
	{
		alSourceStop(m_source);
	}

	void AudioBase::SetLooping(bool bLopping)
	{
		alSourcei(m_source, AL_LOOPING, static_cast<int>(bLopping));
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
			LOG_CRITICAL("Set looping error");
	}

	void AudioBase::SetPitch(float fPitch)
	{
		alSourcef(m_source, AL_PITCH, fPitch);
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
			LOG_CRITICAL("Set pitch error");
	}

	void AudioBase::SetGain(float fGain)
	{
		alSourcef(m_source, AL_GAIN, fGain);
		ALenum error = alGetError();
		if (error != AL_NO_ERROR)
			LOG_CRITICAL("Set gain error");
	}

	void AudioBase::SetPosition(float fX, float fY, float fZ)
	{
		alSource3f(m_source, AL_POSITION, fX, fY, fZ);
	}

	void AudioBase::SetVelocity(float fX, float fY, float fZ)
	{
		alSource3f(m_source, AL_VELOCITY, fX, fY, fZ);
	}

	AudioBase::~AudioBase()
	{
		alDeleteSources(1, &m_source);
		alDeleteBuffers(1, &m_buffer);
	}
}
