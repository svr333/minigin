
#include "EventManager.h"

dae::EventManager::EventManager()
{
	m_Events.reserve(MAX_EVENTS);
}

dae::EventManager::~EventManager()
{
}

void dae::EventManager::AddListener(BaseEvent::EventType type, const std::function<void(std::shared_ptr<BaseEvent> e)>& listener)
{
	m_Listeners[type].push_back(listener);
}

void dae::EventManager::RemoveListener(BaseEvent::EventType type, const std::function<void(std::shared_ptr<BaseEvent> e)>& listener)
{
	auto it = m_Listeners[type];

	// asked friend for help with the target thing
	auto removeCheck = [&](std::function<void(std::shared_ptr<BaseEvent> e)>& f) { return f.target<void(std::shared_ptr<BaseEvent> e)>() == listener.target<void(std::shared_ptr<BaseEvent> e)>(); };
	it.erase(std::remove_if(it.begin(), it.end(), removeCheck));
}

void dae::EventManager::QueueEvent(std::shared_ptr<BaseEvent> e)
{
	if ((int)m_Events.size() > MAX_EVENTS)
	{
		return;
	}

	m_Events.push_back(e);
}

void dae::EventManager::HandleEvents()
{
	// Has to be initialized (null info)
	std::shared_ptr<BaseEvent> e;

	while (PollEvent(e))
	{
		auto listeners = m_Listeners[e->GetType()];

		// loop over all listeners of that event type
		for (size_t i = 0; i < listeners.size(); i++)
		{
			// check if valid, could technically add autoremover if invalid but lazy
			if (listeners[i])
			{
				listeners[i](e);
			}
		}
	}
}

bool dae::EventManager::PollEvent(std::shared_ptr<BaseEvent>& e, bool removeFromQueue)
{
	if (m_Events.empty())
	{
		return false;
	}

	e = m_Events.front();

	if (removeFromQueue)
	{
		m_Events.erase(m_Events.begin());
	}

	return true;
}
