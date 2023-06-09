#include "ShootComponent.h"

#include <Entities/GameObject.h>

#include <Components/MovementComponent.h>
#include <Components/TextureComponent.h>

#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>

#include <Events/EventManager.h>
#include <Services/ServiceLocator.h>

dae::ShootComponent::ShootComponent(GameObject* pOwner, float bulletSpeed)
	: BaseComponent(pOwner)
	, m_BulletSpeed(bulletSpeed)
{
	m_Bullets.reserve(m_MaxBullets);

	auto func = std::bind(&ShootComponent::OnObjectDestroyed, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(BaseEvent::EventType::OBJECT_DESTROYED_EVENT, func);
}

dae::ShootComponent::~ShootComponent() = default;

void dae::ShootComponent::Update(float /*deltaTime*/)
{
}

void dae::ShootComponent::Render() const
{
}

void dae::ShootComponent::ShootBullet()
{
	if ((int)m_Bullets.size() >= m_MaxBullets)
	{
		return;
	}

	auto bullet = std::make_shared<GameObject>();
	bullet->GetTransform().SetLocalPosition(m_pOwner->GetTransform().GetWorldPosition());
	bullet->GetTransform().SetScale({ 0.3f, 0.3f, 1 });

	bullet->AddComponent(new MovementComponent(bullet.get(), {0, -1, 0}, m_BulletSpeed));
	bullet->AddComponent(new TextureComponent(bullet.get(), "Bullet.png"));

	ServiceLocator::GetAudio()->QueueSound(Audio::Sound { Audio::SoundType::ShootBullet, 10 });
	SceneManager::GetInstance().GetActiveScene()->Add(bullet);
	m_Bullets.push_back(bullet);
}

void dae::ShootComponent::OnObjectDestroyed(const BaseEvent& e)
{
	for (int i = 0; i < (int)m_Bullets.size(); i++)
	{
		if (m_Bullets[i].get() == e.GetOwner())
		{
			// new in C++ 20 (looks a lot cleaner)
			std::erase(m_Bullets, m_Bullets[i]);

			m_MissedBullets++;
		}
	}
}
