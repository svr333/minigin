#pragma once
#include "BaseComponent.h"

#include <string>

#include "../Entities/GameObject.h"
#include "../Entities/Texture2D.h"

namespace dae
{
	class TextureComponent final : public BaseComponent
	{
	public:
		TextureComponent(GameObject* pOwner, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

		void Update(float deltaTime) override;
		void Render() const override;

		void SetTexture(const std::string& filename);
		glm::ivec2 GetTextureSize() { return m_pTexture->GetSize(); };

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
