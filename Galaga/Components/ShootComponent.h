#pragma once
#include <Components/BaseComponent.h>
#include <Entities/GameObject.h>

namespace dae
{
	class BaseEvent;

	class ShootComponent final : public BaseComponent
	{
	public:
		ShootComponent(GameObject* pOwner, float bulletSpeed = 300.0f);
		~ShootComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		int GetHitBullets() { return m_HitBullets; };
		int GetMissedBullets() { return m_MissedBullets; };

		bool TryShootBullet(glm::vec3 shootDir = { 0, -1, 0});
		void OnObjectDestroyed(std::shared_ptr<BaseEvent> e);
		void OnCollisionDetected(std::shared_ptr<BaseEvent> e);

	private:
		int m_HitBullets = 0;
		int m_MissedBullets = 0;

		float m_BulletSpeed;
		const int m_MaxBullets = 2;

		std::vector<std::shared_ptr<GameObject>> m_Bullets;

		bool ObjectHit(std::shared_ptr<GameObject> obj);
	};
}
