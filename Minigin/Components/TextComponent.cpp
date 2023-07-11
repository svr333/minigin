#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "../Services/Renderer.h"
#include "../Entities/Font.h"
#include "../Entities/Texture2D.h"

dae::TextComponent::TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& colour)
	: BaseComponent(pOwner)
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_Font(std::move(font))
	, m_Texture(nullptr)
	, m_Colour(colour)
{ }

void dae::TextComponent::Update(float /*deltaTime*/)
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_Colour);

		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}

		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);

		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}

		SDL_FreeSurface(surf);
		m_Texture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void dae::TextComponent::Render() const
{
	if (m_Texture)
	{
		const auto& pos = m_pOwner->GetTransform().GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

// This implementation uses the "dirty flag" pattern
void dae::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void dae::TextComponent::SetColor(const SDL_Color& color)
{
	m_Colour = color;
	m_NeedsUpdate = true;
}
