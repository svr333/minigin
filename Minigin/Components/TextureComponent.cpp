#include "TextureComponent.h"
#include "../Entities/GameObject.h"
#include "../Services/Renderer.h"
#include "../Services/ResourceManager.h"
#include "../Entities/Texture2D.h"

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
	if (!m_pTexture)
	{
		return;
	}

	const auto& pos = m_pOwner->GetTransform().GetWorldPosition();
	const auto& scale = m_pOwner->GetTransform().GetScale();
	const auto& size = m_pTexture->GetSize();

	Renderer::GetInstance().RenderTexture(*m_pTexture, pos.x, pos.y, size.x * scale.x, size.y * scale.y);
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}
