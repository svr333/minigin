#include "PointsComponent.h"
#include <Events/EventManager.h>
#include <Events/Events.h>

dae::PointsComponent::PointsComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
}

void dae::PointsComponent::IncreasePoints(int points)
{
	m_Points += points;

	EventManager::GetInstance().QueueEvent(std::make_shared<PointsUpdatedEvent>(m_pOwner, m_Points));
}

void dae::PointsComponent::Update(float /*deltaTime*/)
{
}

void dae::PointsComponent::Render() const
{
}
