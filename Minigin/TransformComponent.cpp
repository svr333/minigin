#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
}

const glm::vec3& dae::TransformComponent::GetWorldPosition()
{
	if (m_IsDirty)
	{
		m_IsDirty = false;

		m_WorldPosition = m_pOwner->GetParent()
			? m_pOwner->GetParent()->GetTransform().GetWorldPosition() + m_LocalPosition
			: m_LocalPosition;
	}

	return m_WorldPosition;
}

const glm::vec3& dae::TransformComponent::GetLocalPosition() const
{
	return m_LocalPosition;
}

void dae::TransformComponent::SetPositionDirty()
{
	m_IsDirty = true;
	auto children = m_pOwner->GetChildren();

	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->GetTransform().SetPositionDirty();
	}
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& position)
{
	m_LocalPosition = position;
	SetPositionDirty();
}
