#pragma once
#include <Components/BaseComponent.h>
#include "../Entities/Enemy.h"

#include <memory>
#include <glm/glm.hpp>

namespace dae
{
    class BaseEvent;

    // I considered making separate components for each enemy, but because there is only 3, this should be fine
    class EnemyComponent : public BaseComponent
    {
    public:
        enum EnemyState
        {
            Formation = 0,
            Diving = 1,
            Stationing = 2,
            Returning = 3,
            TractorBeam = 4
        };

        EnemyComponent(GameObject* pOwner, Enemy enemy, float playerHeight);

        virtual void Update(float deltaTime) override;
        virtual void Render() const override;
        
        void OnLivesUpdated(std::shared_ptr<BaseEvent> e);
        int GetPointsForHit();

    private:
        Enemy m_Enemy;
        EnemyState m_State = EnemyState::Formation;

        // all variables regarding moving
        float m_CurrentTime = 8.0f;

        // diving
        int m_DiveShotCount = 0;

        glm::vec3 m_DesiredLocation{};
        float m_RequiredSpeed = 0.0f;

        // constants (tweak these for better game experience)
        const int m_BreakFormationPercentChance = 20;
        const float m_FormationTime = 12.0f;

        const float m_DiveTime = 4.0;
        const float m_StationTime = 2.0;
        const float m_ReturnTime = 2.5;
        const float m_TractorTime = 12.0f;

        // this value is used to make the above times a bit more random (except formationtime)
        float m_CurrentActionTime = 0.0f;
        const float m_Deviation = 1.0f;

        const int m_MaxDiveShots = 2;
        const float m_PlayerHeight = 0.0f;

        void DoFormation(float deltaTime);
        void DoDive(float deltaTime);
        void DoStationing(float deltaTime);
        void DoReturn(float deltaTime);
        void DoTractorBeam(float deltaTime);

        float GetRandomDeviationTime();
    };
}
