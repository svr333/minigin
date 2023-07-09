#include "LivesComponent.h"
#include "../Events/EventManager.h"
#include "../Events/Events.h"

dae::LivesComponent::LivesComponent(GameObject* pOwner, int lives)
	: BaseComponent(pOwner)
	, m_TotalLives(lives)
	, m_CurrentLives(lives)
{
	// convert code function to std::function
	auto func = std::bind(&LivesComponent::OnObjectDiedEvent, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(BaseEvent::EventType::OBJECT_DIED_EVENT, func);
}

void dae::LivesComponent::Update(float /*deltaTime*/)
{
}

void dae::LivesComponent::Render() const
{
}

void dae::LivesComponent::OnObjectDiedEvent(const BaseEvent& e)
{
	if (!e.GetOwner() || e.GetOwner() != m_pOwner)
	{
		return;
	}

	m_CurrentLives--;
}
