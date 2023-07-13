#pragma once
#include <Components/BaseComponent.h>
#include <Components/LivesComponent.h>
#include <vector>
#include <memory>
#include "../Entities/Highscore.h"

namespace dae
{
	class HighscoresComponent : public BaseComponent
	{
	public:
		HighscoresComponent(GameObject* pOwner);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		void OnHighscoreAdded(const BaseEvent& e);

	private:
		bool m_IsDirty = false;
		std::vector<Highscore> m_Highscores {};

		std::vector<std::shared_ptr<GameObject>> m_Objects {};

		void GenerateHighscoreObjects();
	};
}
