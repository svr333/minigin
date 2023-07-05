#pragma once
#include "Singleton.h"
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

		void AddListener(BaseEvent::EventType type, const std::function<void(const BaseEvent& e)>& listener);
		void RemoveListener(BaseEvent::EventType type, const std::function<void(const BaseEvent& e)>& listener);

		// event is a C++ keyword so I cant use it as param name (is there a built-in event system?)
		void QueueEvent(const BaseEvent& e);
		void HandleEvents();

		// bool is unused but can be useful later on for manual polling + automatic
		bool PollEvent(BaseEvent& e, bool removeFromQueue = true);

	private:
		std::vector<BaseEvent> m_Events;
		std::unordered_map<BaseEvent::EventType, std::vector<std::function<void(const BaseEvent& e)>>> m_Listeners;

		const int MAX_EVENTS = 100;
	};
}
