#include "TransformComponent.h"

dae::TransformComponent::TransformComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
	, m_Position{}
	, m_Rotation{}
	, m_Scale{}
{
}
