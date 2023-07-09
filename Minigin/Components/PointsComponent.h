#pragma once
#include "BaseComponent.h"

namespace dae
{
	class PointsComponent final : public BaseComponent
	{
	public:
		PointsComponent(GameObject* pOwner);
		void IncreasePoints(int points) { m_Points += points; };

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

	private:
		int m_Points = 0;
	};
}
