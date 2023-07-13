#pragma once
#include "../Entities/GameObject.h"

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
			OBJECT_DESTROYED_EVENT = 2,
			LIVES_UPDATED_EVENT = 3,
			POINTS_UPDATED_EVENT = 4,
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

	class ObjectDestroyedEvent : public BaseEvent
	{
	public:
		ObjectDestroyedEvent(GameObject* pOwner) : BaseEvent(pOwner)
		{
			m_Type = BaseEvent::EventType::OBJECT_DESTROYED_EVENT;
		}
	};

	class LivesUpdatedEvent : public BaseEvent
	{
	public:
		LivesUpdatedEvent(GameObject* pOwner, int newLives) : BaseEvent(pOwner), m_NewLives(newLives)
		{
			m_Type = BaseEvent::EventType::LIVES_UPDATED_EVENT;
		}

	private:
		int m_NewLives;
	};

	class PointsUpdatedEvent : public BaseEvent
	{
	public:
		PointsUpdatedEvent(GameObject* pOwner, int newPoints) : BaseEvent(pOwner), m_NewPoints(newPoints)
		{
			m_Type = BaseEvent::EventType::POINTS_UPDATED_EVENT;
		}

	private:
		int m_NewPoints;
	};
}
