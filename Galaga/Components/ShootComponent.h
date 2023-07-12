#pragma once
#include <Components/BaseComponent.h>
#include <Entities/GameObject.h>

namespace dae
{
	class ShootComponent : public BaseComponent
	{
	public:
		ShootComponent(GameObject* pOwner, float bulletSpeed = 100.0f);
		~ShootComponent();

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void ShootBullet();

	private:
		float m_BulletSpeed;
		const int m_MaxBullets = 2;

		std::vector<std::shared_ptr<GameObject>> m_Bullets;
	};
}
