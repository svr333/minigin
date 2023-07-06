#pragma once
#include "Audio.h"

namespace dae
{
	class ServiceLocator
	{
	public:
		static Audio* GetAudio();
		static void SetAudio(Audio* audio);

		static void Quit();

	private:
		static Audio* m_AudioService;
		static NullAudio m_NullAudioService;
	};
}
