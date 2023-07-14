#include "ShootCommand.h"
#include <Entities/GameObject.h>
#include "../Components/ShootComponent.h"
#include <Services/ServiceLocator.h>

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
		if (comp->TryShootBullet())
		{
			ServiceLocator::GetAudio()->QueueSound(Audio::Sound { Audio::SoundType::ShootBullet, 10 });
		}
	}
}
