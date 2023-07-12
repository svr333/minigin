#include "ShootComponent.h"

#include <Entities/GameObject.h>

#include <Components/MovementComponent.h>
#include <Components/TextureComponent.h>

#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>

dae::ShootComponent::ShootComponent(GameObject* pOwner, float bulletSpeed)
	: BaseComponent(pOwner)
	, m_BulletSpeed(bulletSpeed)
{
	m_Bullets.reserve(m_MaxBullets);
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

	SceneManager::GetInstance().GetActiveScene()->Add(bullet);
	m_Bullets.push_back(bullet);
}
