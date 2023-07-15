#include "DisplayPointsComponent.h"
#include <Events/EventManager.h>
#include <Events/Events.h>
#include <Components/LivesComponent.h>
#include <Components/TextComponent.h>
#include "PointsComponent.h"

dae::DisplayPointsComponent::DisplayPointsComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
	auto func = std::bind(&DisplayPointsComponent::OnPointsUpdated, this, std::placeholders::_1);
	EventManager::GetInstance().AddListener(dae::BaseEvent::EventType::POINTS_UPDATED_EVENT, func);
}

void dae::DisplayPointsComponent::Update(float /*deltaTime*/)
{
}

void dae::DisplayPointsComponent::Render() const
{
}

void dae::DisplayPointsComponent::OnPointsUpdated(std::shared_ptr<BaseEvent> e)
{
	// very similar to health and frames comp, thought briefly about abstracting this into HUDComponent
	// but seemed rather hard to get custom info and thought it wasnt worth it with the time constraint
	auto comp = e->GetOwner()->GetComponent<PointsComponent>();

	if (!comp)
	{
		return;
	}

	auto text = m_pOwner->GetComponent<TextComponent>();

	if (!text)
	{
		return;
	}

	text->SetText("Points: " + std::to_string(comp->GetPoints()));
}
