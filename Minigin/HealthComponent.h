#pragma once
#include "BaseComponent.h"
namespace dae
{
	class HealthComponent final : public BaseComponent
	{
	public:
		HealthComponent(GameObject* pOwner, float maxHealth = 100);
		virtual ~HealthComponent() = default;
		void TakeDamage(float damage);

		void Update(float deltaTime) override;
		void Render() const override;

	private:
		float m_CurrentHealth = 100.0f;
		float m_MaxHealth = 100.0f;
	};
}