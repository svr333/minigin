#include "ServiceLocator.h"

// https://stackoverflow.com/questions/16049306/error-lnk2001-unresolved-external-symbol-private-static-class
// using singleton genuinely feels better (could even add a provide method to turn a singleton into a sort of locator)
NullAudio dae::ServiceLocator::m_NullAudioService{};
Audio* dae::ServiceLocator::m_AudioService = nullptr;

Audio* dae::ServiceLocator::GetAudio()
{
	if (!m_AudioService)
	{
		return &m_NullAudioService;
	}

	return m_AudioService;
}

void dae::ServiceLocator::SetAudio(Audio* audio)
{
	if (m_AudioService)
	{
		delete m_AudioService;
	}

	m_AudioService = audio;
}

void dae::ServiceLocator::Quit()
{
	delete m_AudioService;
}
