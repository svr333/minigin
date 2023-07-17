#pragma once
#include <Components/BaseComponent.h>

#include <memory>

namespace dae
{
	class BaseEvent;

	class DisplayPointsComponent final : public BaseComponent
	{
	public:
		DisplayPointsComponent(GameObject* pOwner, std::shared_ptr<GameObject> pActor);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void OnPointsUpdated(std::shared_ptr<BaseEvent> e);

	private:
		std::shared_ptr<GameObject> m_pActor;
	};
}
