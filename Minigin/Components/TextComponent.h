#pragma once
#include "BaseComponent.h"

#include <string>
#include <memory>
#include <SDL.h>

#include "../Entities/GameObject.h"
#include "../Entities/Transform.h"
#include "../Entities/Texture2D.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(GameObject* pOwner, const std::string& text, std::shared_ptr<Font> font, const SDL_Color& colour = { 255, 255, 255 });
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		void Update(float deltaTime) override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetColor(const SDL_Color& color);

		glm::ivec2 GetTextureSize() { return m_Texture->GetSize(); };

	private:
		bool m_NeedsUpdate;
		SDL_Color m_Colour;

		std::string m_Text;
		std::shared_ptr<Font> m_Font;

		std::shared_ptr<Texture2D> m_Texture;
	};
}
