#include "HealthComponent.h"
#include "../Events/EventManager.h"
#include "../Events/Events.h"

dae::HealthComponent::HealthComponent(GameObject* pOwner, float maxHealth)
	: BaseComponent(pOwner)
	, m_MaxHealth(maxHealth)
	, m_CurrentHealth(maxHealth)
{
}

void dae::HealthComponent::TakeDamage(float damage)
{
	m_CurrentHealth = std::max(m_CurrentHealth - damage, 0.0f);
}

void dae::HealthComponent::Update(float /*deltaTime*/)
{
	// use epsilon to check for death
	if (m_CurrentHealth <= FLT_EPSILON)
	{
		EventManager::GetInstance().QueueEvent(ObjectDiedEvent{ m_pOwner });
	}
}

void dae::HealthComponent::Render() const
{
}
