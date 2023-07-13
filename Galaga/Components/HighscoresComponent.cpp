#include "HighscoresComponent.h"
#include <Events/EventManager.h>
#include <Events/Events.h>
#include <Entities/GameObject.h>
#include "../Json/JsonConvert.h"
#include <Components/TextComponent.h>
#include <Services/ResourceManager.h>
#include <Scenes/SceneManager.h>
#include <Scenes/Scene.h>

// should probably abstract highscore stuff into a service because this is transient and it reads/writes to file
dae::HighscoresComponent::HighscoresComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
	auto func = std::bind(&HighscoresComponent::OnHighscoreAdded, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(BaseEvent::EventType::OBJECT_DESTROYED_EVENT, func);

	m_Highscores = JsonConvert::GetInstance().DeserializeHighscores();
	GenerateHighscoreObjects();
}

void dae::HighscoresComponent::Update(float /*deltaTime*/)
{
	if (m_IsDirty)
	{
		for (int i = 0; i < (int)m_Objects.size(); i++)
		{
			m_Objects[i]->MarkDestroy();
		}

		m_Objects.clear();

		GenerateHighscoreObjects();
	}
}

void dae::HighscoresComponent::Render() const
{
}

void dae::HighscoresComponent::OnHighscoreAdded(const BaseEvent& e)
{
	const auto& highScoreEvent = dynamic_cast<const HighscoreAddedEvent&>(e);

	m_Highscores.push_back(Highscore{ highScoreEvent.GetName(), highScoreEvent.GetScore() });
}

void dae::HighscoresComponent::GenerateHighscoreObjects()
{
	// sort scores first
	auto sort = [](Highscore a, Highscore b) { return a.Score < b.Score; };
	std::sort(m_Highscores.begin(), m_Highscores.end(), sort);

	// maximum amount of 7 scores
	auto amount = std::min((int)m_Highscores.size(), 7);

	for (int i = 0; i < amount; i++)
	{
		auto obj = std::make_shared<GameObject>();

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 48);
		auto text = m_Highscores[i].Name + ": " + std::to_string(m_Highscores[i].Score);
		auto color = i == 0 ? SDL_Color{ 232, 217, 16 }
			: i == 1 ? SDL_Color{ 171, 198, 191 }
			: i == 2 ? SDL_Color{ 142, 109, 37 }
			: SDL_Color{ 206, 206, 206 };

		obj->AddComponent(new TextComponent(obj.get(), text, font, color));
		obj->GetTransform().SetLocalPosition({ 250, 40 + i * 60, 0 });

		SceneManager::GetInstance().GetActiveScene()->Add(obj);
	}
}
