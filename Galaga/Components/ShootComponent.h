#pragma once
#include <Components/BaseComponent.h>
#include <Entities/GameObject.h>

namespace dae
{
	class BaseEvent;

	class ShootComponent final : public BaseComponent
	{
	public:
		ShootComponent(GameObject* pOwner, float bulletSpeed = 200.0f);
		~ShootComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void ShootBullet();
		void OnObjectDestroyed(const BaseEvent& e);

	private:
		int m_HitBullets = 0;
		int m_MissedBullets = 0;

		float m_BulletSpeed;
		const int m_MaxBullets = 2;

		std::vector<std::shared_ptr<GameObject>> m_Bullets;
	};
}
