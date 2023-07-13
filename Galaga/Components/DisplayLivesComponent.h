#pragma once
#include <Components/BaseComponent.h>

namespace dae
{
	class BaseEvent;

	class DisplayLivesComponent final : public BaseComponent
	{
	public:
		DisplayLivesComponent(GameObject* pOwner);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void OnLivesUpdated(const BaseEvent& e);
	};
}
