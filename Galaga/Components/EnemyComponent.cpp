#include "EnemyComponent.h"

#include <Components/TextureComponent.h>
#include <Components/HealthComponent.h>
#include <Components/LivesComponent.h>
#include <Components/CollisionComponent.h>
#include "ShootComponent.h"

#include <Events/EventManager.h>
#include <Entities/GameObject.h>
#include <Scenes/Scene.h>


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

		// create tractor object
		auto tractorBeam = std::make_shared<GameObject>();
		m_pTractorBeam = tractorBeam.get();

		tractorBeam->SetVisible(false);
		tractorBeam->AddComponent(new TextureComponent(tractorBeam.get(), "TractorBeam.png"));
		tractorBeam->AddComponent(new CollisionComponent(tractorBeam.get()));

		tractorBeam->GetTransform().SetLocalPosition({ enemy.X - 12, enemy.Y + 25, 0 });

		tractorBeam->SetParent(m_pOwner);
		m_pOwner->GetScene()->Add(tractorBeam);

		// add callback for lives updated (texture change)
		auto func = std::bind(&EnemyComponent::OnLivesUpdated, this, std::placeholders::_1);
		EventManager::GetInstance().AddListener(BaseEvent::EventType::LIVES_UPDATED_EVENT, func);
	}

	m_pOwner->AddComponent(new HealthComponent(m_pOwner));
	m_pOwner->AddComponent(new TextureComponent(m_pOwner, texturePath));
	m_pOwner->AddComponent(new LivesComponent(m_pOwner, lives));
}

dae::EnemyComponent::~EnemyComponent()
{
	// children dont get automatically deleted

	if (m_pTractorBeam)
	{
		m_pTractorBeam->MarkDestroy();
	}
}

void dae::EnemyComponent::Update(float deltaTime)
{
	if (m_IsAutomomous)
	{
		return;
	}

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
	case dae::EnemyComponent::TractorDive:
		DoTractorDive(deltaTime);
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

	if (m_Enemy.Type == EnemyType::Boss && random < (m_BreakFormationPercentChance / 2))
	{
		DoTractorDive(deltaTime);
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

	// failed to initiate dive
	if (m_State != EnemyState::Diving)
	{
		return;
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
		auto randomY = m_PlayerHeight + (rand() % 45) - 22;

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

void dae::EnemyComponent::DoTractorDive(float deltaTime)
{
	if (m_State == EnemyState::Formation)
	{
		m_State = EnemyState::TractorDive;
		m_CurrentTime = 0.0f;
		m_CurrentActionTime = m_TractorDive + GetRandomDeviationTime();
	}

	// failed to initiate tractorDive
	if (m_State != EnemyState::TractorDive)
	{
		return;
	}

	m_CurrentTime += deltaTime;

	// move to tractor beam
	if (m_CurrentTime > m_CurrentActionTime)
	{
		m_CurrentTime = 0.0f;
		m_DesiredLocation = glm::vec3{};
		m_State = EnemyState::TractorBeam;
		m_CurrentActionTime = m_TractorBeamTime + GetRandomDeviationTime();
		return;
	}

	if (m_DesiredLocation == glm::vec3{})
	{
		// playerHeight - 60 is the distance for tractorbeam to hit player (texture is 80pixels)
		auto randomX = m_Enemy.X + (rand() % 251) - 125;

		m_DesiredLocation = glm::vec3{ randomX, m_PlayerHeight - 60, 0 };
		m_RequiredSpeed = glm::distance(m_DesiredLocation, m_pOwner->GetTransform().GetLocalPosition()) / m_CurrentActionTime;
	}

	auto moveVector = glm::normalize(m_DesiredLocation - m_pOwner->GetTransform().GetLocalPosition()) * m_RequiredSpeed * deltaTime;
	m_pOwner->GetTransform().SetLocalPosition(m_pOwner->GetTransform().GetLocalPosition() + moveVector);
}

void dae::EnemyComponent::DoTractorBeam(float deltaTime)
{
	m_CurrentTime += deltaTime;

	m_pTractorBeam->SetVisible(true);

	// go to returning
	if (m_CurrentTime > m_CurrentActionTime)
	{
		m_CurrentTime = 0.0f;
		m_State = EnemyState::Returning;
		m_CurrentActionTime = m_ReturnTime + GetRandomDeviationTime();
		m_pTractorBeam->SetVisible(false);
		return;
	}
}

float dae::EnemyComponent::GetRandomDeviationTime()
{
	return static_cast<float>(rand()) / (RAND_MAX / m_Deviation);
}
