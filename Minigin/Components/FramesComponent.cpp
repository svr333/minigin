#include "FramesComponent.h"
#include "../Entities/GameObject.h"
#include "TextComponent.h"

dae::FramesComponent::FramesComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
}

void dae::FramesComponent::Update(float deltaTime)
{
	// maybe change to use event system?
	m_FPS = static_cast<int>(1 / deltaTime);
	m_TextTimer += deltaTime;

	if (m_TextTimer >= m_MaxTextTime)
	{
		auto text = m_pOwner->GetComponent<TextComponent>();

		if (text)
		{
			text->SetText(std::to_string(m_FPS) + " FPS");
		}

		m_TextTimer = 0.f;
	}
}

void dae::FramesComponent::Render() const
{
}
