#include "ShootComponent.h"

#include <Entities/GameObject.h>

#include <Components/MovementComponent.h>
#include <Components/TextureComponent.h>
#include <Components/CollisionComponent.h>
#include <Components/HealthComponent.h>
#include "PointsComponent.h"
#include "EnemyComponent.h"

#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>

#include <Events/EventManager.h>
#include <Components/LivesComponent.h>

dae::ShootComponent::ShootComponent(GameObject* pOwner, float bulletSpeed)
	: BaseComponent(pOwner)
	, m_BulletSpeed(bulletSpeed)
{
	m_Bullets.reserve(m_MaxBullets);

	auto objDestroyed = std::bind(&ShootComponent::OnObjectDestroyed, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(BaseEvent::EventType::OBJECT_DESTROYED_EVENT, objDestroyed);

	auto collision = std::bind(&ShootComponent::OnCollisionDetected, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(BaseEvent::EventType::COLLISION_EVENT, collision);
}

dae::ShootComponent::~ShootComponent() = default;

void dae::ShootComponent::Update(float /*deltaTime*/)
{
}

void dae::ShootComponent::Render() const
{
}

bool dae::ShootComponent::TryShootBullet(glm::vec3 shootDir)
{
	if ((int)m_Bullets.size() >= m_MaxBullets)
	{
		return false;
	}

	auto bullet = std::make_shared<GameObject>();
	
	auto texture = new TextureComponent(bullet.get(), "Bullet.png");
	bullet->AddComponent(texture);
	bullet->AddComponent(new MovementComponent(bullet.get(), shootDir, m_BulletSpeed));
	bullet->AddComponent(new CollisionComponent(bullet.get()));

	// a lot of math just to position the thing above the actual object so it doesnt overlap at the start
	auto scale = glm::vec3{ 0.15f, 0.3f, 1 };
	bullet->GetTransform().SetScale(scale);

	auto ownerSize = m_pOwner->GetComponent<TextureComponent>()->GetTextureSize().x * m_pOwner->GetTransform().GetScale().x;
	auto imageSizeX = (glm::float32((texture->GetTextureSize().x * scale.x)));
	auto imageSizeY = (glm::float32((texture->GetTextureSize().y * scale.y)));

	bullet->GetTransform().SetLocalPosition(m_pOwner->GetTransform().GetWorldPosition() + (shootDir * imageSizeY) + glm::vec3{ ownerSize / 2 - imageSizeX / 2, 0, 0 });

	SceneManager::GetInstance().GetActiveScene()->Add(bullet);
	m_Bullets.push_back(bullet);
	return true;
}

void dae::ShootComponent::OnObjectDestroyed(std::shared_ptr<BaseEvent> e)
{
	for (int i = 0; i < (int)m_Bullets.size(); i++)
	{
		if (m_Bullets[i].get() == e->GetOwner())
		{
			// new in C++ 20 (looks a lot cleaner)
			std::erase(m_Bullets, m_Bullets[i]);

			m_MissedBullets++;
		}
	}
}

void dae::ShootComponent::OnCollisionDetected(std::shared_ptr<BaseEvent> e)
{
	// m_pOwner is the object who shot, e->GetOwner() is the bullet, objects is what was hit
	auto collisionEvent = dynamic_cast<CollisionEvent*>(e.get());
	
	auto a = 3;
	a; collisionEvent;

	auto objs = collisionEvent->GetObjects();

	// put as max one cause we can only kill 1 dude with a bullet
	for (int i = 0; i < 1; i++)
	{
		// ignore self hit
		if (objs[i].get() == m_pOwner)
		{
			continue;
		}

		// ignore when the obj isnt one of our bullets
		auto result = std::find_if(m_Bullets.begin(), m_Bullets.end(), [=](std::shared_ptr<GameObject> ptr) { return ptr.get() == e->GetOwner(); });

		if (result == m_Bullets.end())
		{
			continue;
		}

		std::erase_if(m_Bullets, [=](std::shared_ptr<GameObject> obj) { return obj.get() == e->GetOwner(); });

		// check if objs[i] is an object that can take damage
		if (ObjectHit(objs[i]))
		{
			e->GetOwner()->MarkDestroy();
		}
	}
}

bool dae::ShootComponent::ObjectHit(std::shared_ptr<GameObject> obj)
{
	// only player has pointcomp
	auto points = obj->GetComponent<PointsComponent>();
	auto enemy = m_pOwner->GetComponent<EnemyComponent>();
	auto health = obj->GetComponent<HealthComponent>();

	// player got hit by enemy
	if (points && enemy && health)
	{
		health->TakeDamage(5000.0f);
		
		return true;
	}

	points = m_pOwner->GetComponent<PointsComponent>();
	enemy = obj->GetComponent<EnemyComponent>();
	health = obj->GetComponent<HealthComponent>();

	// enemy got hit by player
	if (points && enemy && health)
	{
		health->TakeDamage(5000.0f);

		// get points from EnemyComponent later
		points->IncreasePoints(50);
		
		return true;
	}

	return false;
}
