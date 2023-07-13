#include "DisplayLivesComponent.h"
#include <Events/EventManager.h>
#include <Events/Events.h>
#include <Components/LivesComponent.h>
#include <Components/TextComponent.h>

dae::DisplayLivesComponent::DisplayLivesComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
	auto func = std::bind(&DisplayLivesComponent::OnLivesUpdated, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(dae::BaseEvent::EventType::LIVES_UPDATED_EVENT, func);
}

void dae::DisplayLivesComponent::Update(float /*deltaTime*/)
{
}

void dae::DisplayLivesComponent::Render() const
{
}

void dae::DisplayLivesComponent::OnLivesUpdated(const BaseEvent& e)
{
	auto comp = e.GetOwner()->GetComponent<LivesComponent>();

	if (!comp)
	{
		return;
	}

	auto text = m_pOwner->GetComponent<TextComponent>();

	if (!text)
	{
		return;
	}

	text->SetText("Lives: " + comp->GetLivesLeft());
}
