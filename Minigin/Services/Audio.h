#pragma once
#include <string>
#include <mutex>
#include <queue>
#include <future>

class Audio
{
public:
	virtual ~Audio() {};

	// not necessary but cleaner overview of sounds
	enum SoundType
	{
		BackgroundMusic = 0
	};

	struct Sound
	{
		SoundType Type;
		int Volume = 20;
		int Repeat = 0;
	};

	virtual void QueueSound(Sound sound) = 0;
	virtual void InitializeSound(const std::string& path, SoundType sound) = 0;
	virtual void HandleSounds() = 0;
};

class AudioPlayer final : public Audio
{
public:
	AudioPlayer(const std::string& rootPath);
	~AudioPlayer();

	virtual void QueueSound(Sound sound) override;
	virtual void InitializeSound(const std::string& path, SoundType sound) override;
	virtual void HandleSounds() override;

private:
	class SDLAudioImpl;
	SDLAudioImpl* m_pImpl;

	std::queue<Sound> m_SoundQueue;

	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	bool m_Exit = false;
	std::jthread m_Thread;

	//std::vector<std::future<void>> m_Waste {};
};

class NullAudio final : public Audio
{
public:
	virtual void QueueSound(Sound) override {};
	virtual void InitializeSound(const std::string&, SoundType) override {};
	virtual void HandleSounds() {};
};

class LogAudio final : public Audio
{
public:
	LogAudio(Audio* wrapper) : m_Wrapper(wrapper) {};

	virtual void QueueSound(Sound sound) override;
	virtual void InitializeSound(const std::string& path, SoundType sound) override;
	virtual void HandleSounds() override;

private:
	Audio* m_Wrapper;
};
