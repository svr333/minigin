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
	auto newPos = oldPos + m_Direction * m_Speed * deltaTime;

	// dont go out of bounds
	newPos.x = std::max(newPos.x, 10.0f);
	newPos.x = std::min(newPos.x, 600.0f);

	m_pObject->GetTransform().SetLocalPosition(newPos);
}
