#pragma once
#include "BaseComponent.h"

#include <memory>

namespace dae
{
	class BaseEvent;

	class LivesComponent final : public BaseComponent
	{
	public:
		LivesComponent(GameObject* pOwner, int lives = 3);

		void Update(float deltaTime);
		void Render() const;

		int GetLivesLeft() { return m_CurrentLives; };
		int GetTotalLives() { return m_TotalLives; };
		void OnObjectDiedEvent(std::shared_ptr<BaseEvent> e);

	private:
		int m_CurrentLives;
		int m_TotalLives;
	};
}
