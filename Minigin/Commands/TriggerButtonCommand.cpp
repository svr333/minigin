#include "TriggerButtonCommand.h"
#include "../Entities/GameObject.h"
#include "../Components/MenuComponent.h"

dae::TriggerButtonCommand::TriggerButtonCommand(GameObject* pObject)
	: Command()
	, m_pObject(pObject)
{
}

void dae::TriggerButtonCommand::Execute(float /*deltaTime*/)
{
	auto menu = m_pObject->GetComponent<MenuComponent>();

	menu->ExecuteButton();
}
