#pragma once
#include "GameObject.h"

namespace dae
{
	class BaseEvent
	{
	public:
		BaseEvent(GameObject* pOwner) : m_pOwner(pOwner) { };

		enum EventType
		{
			BASE_EVENT = 0,
			OBJECT_DIED_EVENT = 1,
		};

		EventType GetType() const { return m_Type; };
		GameObject* GetOwner() const { return m_pOwner; };

	protected:
		EventType m_Type = EventType::BASE_EVENT;

	private:
		GameObject* m_pOwner;
	};

	// not final in case PlayerDied should inherit
	class ObjectDiedEvent : public BaseEvent
	{
	public:
		ObjectDiedEvent(GameObject* pOwner) : BaseEvent(pOwner)
		{
			m_Type = BaseEvent::EventType::OBJECT_DIED_EVENT;
		}
	};
}
