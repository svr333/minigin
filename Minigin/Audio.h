#pragma once
#include <string>

class Audio
{
public:
	virtual ~Audio() {};

	// not necessary but cleaner overview of sounds
	enum SoundType
	{
		BackgroundMusic = 0
	};

	virtual void PlaySound(SoundType sound, int volume, int repeat = 0) = 0;
	virtual void InitializeSound(const std::string& path, SoundType sound) = 0;
};

class AudioPlayer final : public Audio
{
public:
	AudioPlayer(const std::string& rootPath);
	~AudioPlayer();

	virtual void PlaySound(SoundType sound, int volume, int repeat = 0) override;
	virtual void InitializeSound(const std::string& path, SoundType sound) override;

private:
	class SDLAudioImpl;
	SDLAudioImpl* m_pImpl;
};

class NullAudio final : public Audio
{
public:
	virtual void PlaySound(SoundType, int, int = 0) override {};
	virtual void InitializeSound(const std::string&, SoundType) override {};
};

class LogAudio final : public Audio
{
public:
	LogAudio(Audio* wrapper) : m_Wrapper(wrapper) {};

	virtual void PlaySound(SoundType sound, int volume, int repeat = 0) override;
	virtual void InitializeSound(const std::string& path, SoundType sound) override;

private:
	Audio* m_Wrapper;
};
