#include "CollisionComponent.h"

#include "TextureComponent.h"
#include "TextComponent.h"

#include "../Entities/GameObject.h"
#include "../Scenes/SceneManager.h"
#include "../Scenes/Scene.h"
#include "../Events/EventManager.h"

#include <iostream>

dae::CollisionComponent::CollisionComponent(GameObject* pOwner)
	: BaseComponent(pOwner)
{
	glm::fvec2 size{};

	if (pOwner->HasComponent<TextureComponent>())
	{
		size = pOwner->GetComponent<TextureComponent>()->GetTextureSize();
	}
	else if (pOwner->HasComponent<TextComponent>())
	{
		size = pOwner->GetComponent<TextComponent>()->GetTextureSize();
	}

	auto scale = pOwner->GetTransform().GetScale();
	m_Size = size * glm::fvec2{ scale.x, scale.y };
}

void dae::CollisionComponent::Update(float /*deltaTime*/)
{
	std::vector<std::shared_ptr<GameObject>> objects{};

	if (CheckCollision(objects))
	{
		//std::cout << "Collided with " + std::to_string((int)objects.size()) + " objects\n";
		EventManager::GetInstance().QueueEvent(std::make_shared<CollisionEvent>(m_pOwner, objects));
	}
}

void dae::CollisionComponent::Render() const
{
}

void dae::CollisionComponent::SetBorderDimensions(float width, float height)
{
	m_Size = { width, height };
}

bool dae::CollisionComponent::CheckCollision(std::vector<std::shared_ptr<GameObject>>& m_Objects)
{
	std::vector<std::shared_ptr<GameObject>> sceneObjs = SceneManager::GetInstance().GetActiveScene()->GetObjects();

	const auto pos = m_pOwner->GetTransform().GetWorldPosition();

	const float right = pos.x + m_Size.x;
	const float bottom = pos.y + m_Size.y;

	for (int i = 0; i < (int)sceneObjs.size(); i++)
	{
		// might be better to make a comp to mark objects as collidable
		if (sceneObjs[i].get() == m_pOwner || sceneObjs[i]->GetComponentCount() <= 1 || !sceneObjs[i]->HasComponent<TextureComponent>())
		{
			continue;
		}

		// use texture for size, which is not the best way to do it but good enough for this game
		auto objPos = sceneObjs[i]->GetTransform().GetWorldPosition();
		auto scale = sceneObjs[i]->GetTransform().GetScale();

		// need to convert from ints to floats
		auto textureSize = sceneObjs[i]->GetComponent<TextureComponent>()->GetTextureSize();
		auto size = glm::fvec2{ textureSize.x * scale.x, textureSize.y * scale.y };

		float objRight = objPos.x + size.x;
		float objBottom = objPos.y + size.y;

		bool overlapX = (pos.x <= objRight) && (right >= objPos.x);
		bool overlapY = (pos.y <= objBottom) && (bottom >= objPos.y);

		if (overlapX && overlapY)
		{
			m_Objects.push_back(sceneObjs[i]);
		}
	}

	return !m_Objects.empty();
}
