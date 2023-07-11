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

	void PlaySound(Sound sound);
	void InitializeSound(const std::string& path, SoundType sound);

private:
	std::unordered_map<int, Mix_Chunk*> m_Sounds;
	std::string m_RootPath;
};

AudioPlayer::AudioPlayer(const std::string& rootPath)
{
	m_pImpl = new SDLAudioImpl(rootPath);

	m_Thread = std::jthread(&AudioPlayer::HandleSounds, this);
}

AudioPlayer::~AudioPlayer()
{
	m_Exit = true;
	m_Condition.notify_one();

	delete m_pImpl;
}

void AudioPlayer::QueueSound(Sound sound)
{
	std::unique_lock<std::mutex> lock(m_Mutex);

	m_SoundQueue.push(sound);
	m_Condition.notify_one();
}

void AudioPlayer::InitializeSound(const std::string& path, SoundType sound)
{
	/* if I put the object in local scope, this function will wait until std::async is fulfilled
	 * by putting it in a bigger scope, the function will exist and thus make the program async
	 * putting these in a container without auto destroying is probably terrible, so I will see if I can fix later */
	/*m_Waste.push_back(std::async(std::launch::async, [=]()
	{
		m_pImpl->InitializeSound(path, sound);
	}));*/

	/* this works but gives memory leaks when program is exited while a sound is being initialized(this is not good either)
	// https://stackoverflow.com/questions/16296284/workaround-for-blocking-async (last comment says itll be fine)
	
	std::jthread([=]()
		{
			m_pImpl->InitializeSound(path, sound);
		}).detach();
	*/

	m_pImpl->InitializeSound(path, sound);
}

void AudioPlayer::HandleSounds()
{
	while (!m_Exit)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [=]() { return !m_SoundQueue.empty() || m_Exit; });

		if (m_Exit)
		{
			return;
		}

		auto sound = m_SoundQueue.front();
		m_SoundQueue.pop();

		lock.unlock();

		m_pImpl->PlaySound(sound);
	}
}

AudioPlayer::SDLAudioImpl::SDLAudioImpl(const std::string& rootPath)
{
	m_RootPath = rootPath;

	// setup from yt video
	SDL_Init(SDL_INIT_AUDIO);

	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize audio: %s", Mix_GetError());
	}
}

AudioPlayer::SDLAudioImpl::~SDLAudioImpl()
{
	for (int i = 0; i < (int)m_Sounds.size(); i++)
	{
		if (m_Sounds[i])
		{
			Mix_FreeChunk(m_Sounds[i]);
		}
	}

	m_Sounds.clear();
	Mix_CloseAudio();
}

void AudioPlayer::SDLAudioImpl::PlaySound(Sound sound)
{
	if (sound.Type >= (int)m_Sounds.size() || !m_Sounds[sound.Type])
	{
		std::cout << "Failed to play sound number " << sound.Type << "\n";
		return;
	}

	Mix_VolumeChunk(m_Sounds[sound.Type], sound.Volume);
	Mix_PlayChannel(-1, m_Sounds[sound.Type], sound.Repeat);
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

void LogAudio::QueueSound(Sound sound)
{
	std::cout << "Playing " << sound.Type << " at " << sound.Volume << " volume!\n";
	m_Wrapper->QueueSound(sound);
}

void LogAudio::InitializeSound(const std::string& path, SoundType sound)
{
	std::cout << "Initializing sound with path: " << path << "\n";
	m_Wrapper->InitializeSound(path, sound);
}

void LogAudio::HandleSounds()
{
	// too spammy
	// std::cout << "Initializing sound with path: " << path << "\n";
	m_Wrapper->HandleSounds();
}
