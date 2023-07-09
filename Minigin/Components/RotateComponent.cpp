#include "RotateComponent.h"
#include "../Entities/GameObject.h"

dae::RotateComponent::RotateComponent(GameObject* pOwner, float radius, float rotateSpeed, bool clockwise)
	: BaseComponent(pOwner)
	, m_Radius(radius)
	, m_RotateSpeed(rotateSpeed)
	, m_Clockwise(clockwise)
{
	m_CurrentPos = pOwner->GetTransform().GetLocalPosition();
}

void dae::RotateComponent::Update(float deltaTime)
{
	if (!m_ShouldRotate)
	{
		return;
	}

	auto multiplier = m_Clockwise ? 1 : -1;
	m_Angle += deltaTime * m_RotateSpeed * multiplier;

	glm::vec3 newPos = { m_CurrentPos.x + m_Radius * cosf(m_Angle * 3.14f / 180), m_CurrentPos.y + m_Radius * sinf(m_Angle * 3.14f / 180), 0 };

	m_pOwner->GetTransform().SetLocalPosition(newPos);
}
