#include "LevelComponent.h"

#include "EnemyComponent.h"
#include "../Json/JsonConvert.h"
#include "../Entities/Enemy.h"

#include <Entities/GameObject.h>
#include <Scenes/Scene.h>

dae::LevelComponent::LevelComponent(GameObject* pOwner, const std::string& levelName, std::shared_ptr<GameObject> pPlayer)
	: BaseComponent(pOwner)
	, m_LevelName(levelName)
	, m_pPlayer (pPlayer)
{
	InitializeEnemies();
}

void dae::LevelComponent::InitializeEnemies()
{
	auto enemies = JsonConvert::GetInstance().DeserializeEnemies(m_LevelName);
	
	for (int i = 0; i < (int)enemies.size(); i++)
	{
		auto obj = std::make_shared<GameObject>();

		obj->GetTransform().SetLocalPosition({ enemies[i].X, enemies[i].Y, 0 });
		obj->GetTransform().SetScale({ 0.55f, 0.55f, 1 });

		obj->AddComponent(new EnemyComponent(obj.get(), enemies[i], m_pPlayer->GetTransform().GetWorldPosition().y));

		obj->SetParent(m_pOwner);
		m_pOwner->GetScene()->Add(obj);
	}
}

void dae::LevelComponent::Update(float deltaTime)
{
	// horizontal swing time
	if (m_CurrentHorizontalDirectionLeft)
	{
		m_CurrentHorizontalSwingTime += deltaTime;
	}
	else
	{
		m_CurrentHorizontalSwingTime -= deltaTime;
	}

	if (m_CurrentHorizontalSwingTime >= m_MaxHorizontalSwingTime || m_CurrentHorizontalSwingTime <= -m_MaxHorizontalSwingTime)
	{
		m_CurrentHorizontalDirectionLeft = !m_CurrentHorizontalDirectionLeft;
	}

	// vertical swing time
	if (m_CurrentVerticalDirectionUp)
	{
		m_CurrentVerticalSwingTime -= deltaTime;
	}
	else
	{
		m_CurrentVerticalSwingTime += deltaTime;
	}

	if (m_CurrentVerticalSwingTime >= m_MaxVerticalSwingTime || m_CurrentVerticalSwingTime <= -m_MaxVerticalSwingTime)
	{
		m_CurrentVerticalDirectionUp = !m_CurrentVerticalDirectionUp;
	}

	auto horizontal = sinf(m_CurrentHorizontalSwingTime / (m_MaxHorizontalSwingTime * 2)) * m_MaxHorizontalSwingDistance;
	auto vertical = sinf(m_CurrentVerticalSwingTime / m_MaxVerticalSwingTime / 2) * m_VerticalSwingDistance;
	m_pOwner->GetTransform().SetLocalPosition({ horizontal, vertical, 0 });
}

void dae::LevelComponent::Render() const
{
}
