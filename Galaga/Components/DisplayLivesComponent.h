#pragma once
#include <Components/BaseComponent.h>

#include <memory>

namespace dae
{
	class BaseEvent;

	class DisplayLivesComponent final : public BaseComponent
	{
	public:
		DisplayLivesComponent(GameObject* pOwner);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void OnLivesUpdated(std::shared_ptr<BaseEvent> e);
	};
}
