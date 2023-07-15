#pragma once
#include "../Services/Singleton.h"
#include "Events.h"
#include <vector>
#include <unordered_map>
#include <functional>

namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		EventManager();
		~EventManager();

		void AddListener(BaseEvent::EventType type, const std::function<void(std::shared_ptr<BaseEvent> e)>& listener);
		void RemoveListener(BaseEvent::EventType type, const std::function<void(std::shared_ptr<BaseEvent> e)>& listener);

		// event is a C++ keyword so I cant use it as param name (is there a built-in event system?)
		void QueueEvent(std::shared_ptr<BaseEvent> e);
		void HandleEvents();

		// bool is unused but can be useful later on for manual polling + automatic
		bool PollEvent(std::shared_ptr<BaseEvent>&, bool removeFromQueue = true);

	private:
		std::vector<std::shared_ptr<BaseEvent>> m_Events;
		std::unordered_map<BaseEvent::EventType, std::vector<std::function<void(std::shared_ptr<BaseEvent> e)>>> m_Listeners;

		const int MAX_EVENTS = 100;
	};
}
