#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include <string>

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

	private:
		std::shared_ptr<Texture2D> m_pTexture;
	};
}
