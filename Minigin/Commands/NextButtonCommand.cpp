#include "NextButtonCommand.h"
#include "../Entities/GameObject.h"
#include "../Components/MenuComponent.h"

dae::NextButtonCommand::NextButtonCommand(GameObject* obj, bool previous)
	: Command()
	, m_pObject(obj)
	, m_Previous(previous)
{
}

void dae::NextButtonCommand::Execute(float /*deltaTime*/)
{
	auto menu = m_pObject->GetComponent<MenuComponent>();

	menu->SelectNextButton(m_Previous);
}
