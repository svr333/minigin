#include "Audio.h"
#include <iostream>
#include <SDL_mixer.h>
#include <SDL.h>
#include <unordered_map>
#include <thread>

class AudioPlayer::SDLAudioImpl
{
public:
	SDLAudioImpl(const std::string& rootPath);
	~SDLAudioImpl();

	void PlaySound(SoundType sound, int volume, int repeat = 0);
	void InitializeSound(const std::string& path, SoundType sound);

private:
	std::unordered_map<int, Mix_Chunk*> m_Sounds;
	std::string m_RootPath;
};

AudioPlayer::AudioPlayer(const std::string& rootPath)
{
	m_pImpl = new SDLAudioImpl(rootPath);
}

AudioPlayer::~AudioPlayer()
{
	delete m_pImpl;
}

void AudioPlayer::PlaySound(SoundType sound, int volume, int repeat)
{
	m_pImpl->PlaySound(sound, volume, repeat);
}

void AudioPlayer::InitializeSound(const std::string& path, SoundType sound)
{
	m_pImpl->InitializeSound(path, sound);
}

AudioPlayer::SDLAudioImpl::SDLAudioImpl(const std::string& rootPath)
{
	m_RootPath = rootPath;

	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize audio: %s", Mix_GetError());
	}
}

AudioPlayer::SDLAudioImpl::~SDLAudioImpl()
{
	for (int i = 0; i < m_Sounds.size(); i++)
	{
		if (m_Sounds[i])
		{
			Mix_FreeChunk(m_Sounds[i]);
		}
	}

	m_Sounds.clear();
	Mix_CloseAudio();
}

void AudioPlayer::SDLAudioImpl::PlaySound(SoundType sound, int volume, int repeat)
{
	if (sound >= m_Sounds.size() || !m_Sounds[sound])
	{
		std::cout << "Failed to play sound number " << sound << "\n";
		return;
	}

	Mix_VolumeChunk(m_Sounds[sound], volume);
	Mix_PlayChannel(-1, m_Sounds[sound], repeat);
}

void AudioPlayer::SDLAudioImpl::InitializeSound(const std::string& path, SoundType sound)
{
	Mix_Chunk* chunk = Mix_LoadWAV((m_RootPath + path).c_str());

	if (chunk == nullptr)
	{
		std::cout << "Failed to load sound with path: " << path << "\n";
		return;
	}

	m_Sounds[sound] = chunk;
}

void LogAudio::PlaySound(SoundType sound, int volume, int repeat)
{
	std::cout << "Playing " << sound << " at " << volume << " volume!\n";
	m_Wrapper->PlaySound(sound, volume, repeat);
}

void LogAudio::InitializeSound(const std::string& path, SoundType sound)
{
	std::cout << "Initializing sound with path: " << path << "\n";
	m_Wrapper->InitializeSound(path, sound);
}
