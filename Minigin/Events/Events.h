#pragma once
#include "../Entities/GameObject.h"
#include <string>

namespace dae
{
	class BaseEvent
	{
	public:
		BaseEvent(GameObject* pOwner) : m_pOwner(pOwner) { };
		virtual ~BaseEvent() = default;

		enum EventType
		{
			BASE_EVENT = 0,
			OBJECT_DIED_EVENT = 1,
			OBJECT_DESTROYED_EVENT = 2,
			LIVES_UPDATED_EVENT = 3,
			POINTS_UPDATED_EVENT = 4,
			HIGHSCORE_ADDED = 5,
		};

		EventType GetType() const { return m_Type; };
		GameObject* GetOwner() const { return m_pOwner; };

	protected:
		EventType m_Type = EventType::BASE_EVENT;

	private:
		GameObject* m_pOwner;
	};

	class ObjectDiedEvent final : public BaseEvent
	{
	public:
		ObjectDiedEvent(GameObject* pOwner) : BaseEvent(pOwner)
		{
			m_Type = BaseEvent::EventType::OBJECT_DIED_EVENT;
		}
	};

	class ObjectDestroyedEvent final : public BaseEvent
	{
	public:
		ObjectDestroyedEvent(GameObject* pOwner) : BaseEvent(pOwner)
		{
			m_Type = BaseEvent::EventType::OBJECT_DESTROYED_EVENT;
		}
	};

	class LivesUpdatedEvent final : public BaseEvent
	{
	public:
		LivesUpdatedEvent(GameObject* pOwner, int newLives) : BaseEvent(pOwner), m_NewLives(newLives)
		{
			m_Type = BaseEvent::EventType::LIVES_UPDATED_EVENT;
		}

		const int GetNewLives() { return m_NewLives; };

	private:
		int m_NewLives;
	};

	class PointsUpdatedEvent final : public BaseEvent
	{
	public:
		PointsUpdatedEvent(GameObject* pOwner, int newPoints) : BaseEvent(pOwner), m_NewPoints(newPoints)
		{
			m_Type = BaseEvent::EventType::POINTS_UPDATED_EVENT;
		}

		const int GetNewPoints() { return m_NewPoints; };

	private:
		int m_NewPoints;
	};

	class HighscoreAddedEvent final : public BaseEvent
	{
	public:
		HighscoreAddedEvent(GameObject* pOwner, const std::string& name, int score) : BaseEvent(pOwner), m_Name(name), m_Score(score)
		{
			m_Type = BaseEvent::EventType::HIGHSCORE_ADDED;
		}

		std::string GetName() const { return m_Name; };
		int GetScore() const { return m_Score; };

	private:
		std::string m_Name;
		int m_Score;
	};
}
