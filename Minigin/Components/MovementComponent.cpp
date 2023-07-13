#include "MovementComponent.h"
#include "../Entities/GameObject.h"

dae::MovementComponent::MovementComponent(GameObject* pOwner, const glm::vec3& direction, float speed, float maxDistance)
	: BaseComponent(pOwner)
	, m_Speed(speed)
	, m_Direction(direction)
	, m_MaxDistance(maxDistance)
{
	m_StartPos = m_pOwner->GetTransform().GetLocalPosition();
}

void dae::MovementComponent::Update(float deltaTime)
{
	auto distance = m_Speed * deltaTime;

	if (m_CurrentDistance >= m_MaxDistance)
	{
		m_pOwner->MarkDestroy();
		return;
	}

	m_CurrentDistance += distance;

	auto oldPos = m_pOwner->GetTransform().GetLocalPosition();
	m_pOwner->GetTransform().SetLocalPosition(oldPos + m_Direction * distance);
}

void dae::MovementComponent::Render() const
{
}

bool dae::MovementComponent::IsFinished()
{
	return m_CurrentDistance >= m_MaxDistance;
}
