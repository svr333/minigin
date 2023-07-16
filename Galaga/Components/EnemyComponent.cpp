#include "EnemyComponent.h"

#include <Components/TextureComponent.h>
#include <Components/HealthComponent.h>
#include <Components/LivesComponent.h>
#include "ShootComponent.h"

#include <Events/EventManager.h>
#include <Entities/GameObject.h>


dae::EnemyComponent::EnemyComponent(GameObject* pOwner, Enemy enemy, float playerHeight)
	: BaseComponent(pOwner)
	, m_Enemy(enemy)
	, m_PlayerHeight(playerHeight)
{
	std::string texturePath = enemy.Type == EnemyType::Butterfly ? "Butterfly.png" : "Bee.png";
	int lives = 1;

	if (enemy.Type == EnemyType::Boss)
	{
		texturePath = "Boss.png";
		lives = 2;

		auto func = std::bind(&EnemyComponent::OnLivesUpdated, this, std::placeholders::_1);
		EventManager::GetInstance().AddListener(BaseEvent::EventType::LIVES_UPDATED_EVENT, func);
	}

	m_pOwner->AddComponent(new HealthComponent(m_pOwner));
	m_pOwner->AddComponent(new TextureComponent(m_pOwner, texturePath));
	m_pOwner->AddComponent(new LivesComponent(m_pOwner, lives));
}

void dae::EnemyComponent::Update(float deltaTime)
{
	switch (m_State)
	{
	case dae::EnemyComponent::Formation:
		DoFormation(deltaTime);
		break;
	case dae::EnemyComponent::Diving:
		DoDive(deltaTime);
		break;
	case dae::EnemyComponent::Stationing:
		DoStationing(deltaTime);
		break;
	case dae::EnemyComponent::Returning:
		DoReturn(deltaTime);
		break;
	case dae::EnemyComponent::TractorBeam:
		DoTractorBeam(deltaTime);
		break;
	}
}

void dae::EnemyComponent::Render() const
{
}

void dae::EnemyComponent::OnLivesUpdated(std::shared_ptr<BaseEvent> e)
{
	// this function is only bound on Boss enemy
	if (e->GetOwner() == m_pOwner)
	{
		auto texture = m_pOwner->GetComponent<TextureComponent>();

		texture->SetTexture("Boss_2.png");
	}
}

int dae::EnemyComponent::GetPointsForHit()
{
	int points = 0;

	switch (m_Enemy.Type)
	{
	case EnemyType::Bee:
		points = 50;
		break;
	case EnemyType::Butterfly:
		points = 60;
		break;
	case EnemyType::Boss:
		points = 400;

		if (m_State == EnemyState::Formation)
		{
			points = 150;
		}
		
		return points;
	default:
		break;
	}

	auto multiplier = m_State == EnemyState::Formation ? 1 : 2;
	return points * multiplier;
}

void dae::EnemyComponent::DoFormation(float deltaTime)
{
	m_CurrentTime += deltaTime;

	if (m_CurrentTime < m_FormationTime)
	{
		return;
	}

	m_CurrentTime = 0.0f;
	auto random = rand() % 101;

	if (random >= m_BreakFormationPercentChance)
	{
		return;
	}

	if (m_Enemy.Type == EnemyType::Boss && m_BreakFormationPercentChance < (random / 2))
	{
		DoTractorBeam(deltaTime);
		return;
	}

	DoDive(deltaTime);
}

void dae::EnemyComponent::DoDive(float deltaTime)
{
	// start diving
	if (m_State == EnemyState::Formation)
	{
		m_State = EnemyState::Diving;
		m_CurrentTime = 0.0f;
		m_CurrentActionTime = m_DiveTime + GetRandomDeviationTime();
	}

	m_CurrentTime += deltaTime;

	// go to stationing
	if (m_CurrentTime > m_CurrentActionTime)
	{
		m_CurrentTime = 0.0f;
		m_DesiredLocation = glm::vec3{};
		m_State = EnemyState::Stationing;
		m_CurrentActionTime = m_StationTime + GetRandomDeviationTime();
		return;
	}
	
	// set destination the first time
	if (m_DesiredLocation == glm::vec3{})
	{
		auto randomX = m_Enemy.X + (rand() % 151) - 75;
		auto randomY = m_PlayerHeight + (rand() % 30) - 15;

		m_DesiredLocation = glm::vec3{ randomX, randomY, 0 };
		m_RequiredSpeed = glm::distance(m_DesiredLocation, m_pOwner->GetTransform().GetLocalPosition()) / m_CurrentActionTime;
	}

	auto moveVector = glm::normalize(m_DesiredLocation - m_pOwner->GetTransform().GetLocalPosition()) * m_RequiredSpeed * deltaTime;
	m_pOwner->GetTransform().SetLocalPosition(m_pOwner->GetTransform().GetLocalPosition() + moveVector);
}

void dae::EnemyComponent::DoStationing(float deltaTime)
{
	// moving is already done by the swinging from the level
	// I could actually implement custon code here but itll be fine
	m_CurrentTime += deltaTime;

	// go to returning
	if (m_CurrentTime > m_CurrentActionTime)
	{
		m_CurrentTime = 0.0f;
		m_State = EnemyState::Returning;
		m_CurrentActionTime = m_ReturnTime + GetRandomDeviationTime();
		return;
	}
}

void dae::EnemyComponent::DoReturn(float deltaTime)
{
	m_CurrentTime += deltaTime;

	// go to formation
	if (m_CurrentTime > m_CurrentActionTime)
	{
		m_CurrentTime = 0.0f;
		m_DesiredLocation = glm::vec3{};
		m_State = EnemyState::Formation;
		m_CurrentActionTime = m_FormationTime;

		// without this line they shake a little bit (presumably due to float precision errors)
		m_pOwner->GetTransform().SetLocalPosition({ m_Enemy.X, m_Enemy.Y, 0 });
		return;
	}

	if (m_DesiredLocation == glm::vec3{})
	{
		m_DesiredLocation = glm::vec3{ m_Enemy.X, m_Enemy.Y, 0 };
		m_RequiredSpeed = glm::distance(m_DesiredLocation, m_pOwner->GetTransform().GetLocalPosition()) / m_CurrentActionTime;
	}

	auto moveVector = glm::normalize(m_DesiredLocation - m_pOwner->GetTransform().GetLocalPosition()) * m_RequiredSpeed * deltaTime;
	m_pOwner->GetTransform().SetLocalPosition(m_pOwner->GetTransform().GetLocalPosition() + moveVector);
}

void dae::EnemyComponent::DoTractorBeam(float deltaTime)
{
	if (m_State == EnemyState::Formation)
	{
		m_State = EnemyState::TractorBeam;
	}

	m_CurrentTime = 0.0f;
	deltaTime;
}

float dae::EnemyComponent::GetRandomDeviationTime()
{
	return static_cast<float>(rand()) / (RAND_MAX / m_Deviation);
}
