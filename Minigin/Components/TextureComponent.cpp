#include "TextureComponent.h"
#include "../Entities/GameObject.h"
#include "../Services/Renderer.h"
#include "../Services/ResourceManager.h"

dae::TextureComponent::TextureComponent(GameObject* pOwner, const std::string& filename)
	: BaseComponent(pOwner)
{
	SetTexture(filename);
}

void dae::TextureComponent::Update(float /*deltaTime*/)
{
}

void dae::TextureComponent::Render() const
{
	const auto& pos = m_pOwner->GetTransform().GetWorldPosition();

	if (m_pTexture)
	{
		Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
