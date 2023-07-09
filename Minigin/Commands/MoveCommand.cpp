#include "MoveCommand.h"
#include "../Entities/GameObject.h"

dae::MoveCommand::MoveCommand(GameObject* obj, glm::vec3 direction, float speed)
	: Command()
	, m_pObject(obj)
	, m_Direction(direction)
	, m_Speed(speed)
{
}

void dae::MoveCommand::Execute(float deltaTime)
{
	auto oldPos = m_pObject->GetTransform().GetLocalPosition();
	m_pObject->GetTransform().SetLocalPosition(oldPos + m_Direction * m_Speed * deltaTime);
}
