#include "ShootCommand.h"
#include <Entities/GameObject.h>
#include "../Components/ShootComponent.h"

dae::ShootCommand::ShootCommand(GameObject* pObject)
	: Command()
	, m_pObject(pObject)
{
}

void dae::ShootCommand::Execute(float /*deltaTime*/)
{
	auto comp = m_pObject->GetComponent<ShootComponent>();

	if (comp)
	{
		comp->ShootBullet();
	}
}
