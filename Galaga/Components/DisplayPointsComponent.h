#pragma once
#include <Components/BaseComponent.h>

namespace dae
{
	class BaseEvent;

	class DisplayPointsComponent final : public BaseComponent
	{
	public:
		DisplayPointsComponent(GameObject* pOwner);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void OnPointsUpdated(const BaseEvent& e);
	};
}
