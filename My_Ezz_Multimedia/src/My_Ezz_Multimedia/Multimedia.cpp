#include "My_Ezz_Multimedia/Multimedia.hpp"
#include "My_Ezz_Logger/Log.hpp"
#include "al.h"
#include <alc.h>

namespace My_Ezz
{
	void Multimedia::InitSoundContext()
	{
		ALCdevice* device = alcOpenDevice(nullptr);

		ALboolean enumeration = alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT");
		
		ALCcontext* context;

		context = alcCreateContext(device, nullptr);
		if (!alcMakeContextCurrent(context))
		{
			LOG_CRITICAL("Cannot initialize OpenAL context: {0}", alcGetError(device));
			return;
		}
		
		LOG_INFO("OpenAL context has been initialized successfully!");
		LOG_INFO("OpenAL Vendor: {0}", alGetString(AL_VENDOR));
		LOG_INFO("OpenAL Renderer: {0}", alGetString(AL_RENDERER));
		LOG_INFO("OpenAL Version: {0}", alGetString(AL_VERSION));
	}
}

