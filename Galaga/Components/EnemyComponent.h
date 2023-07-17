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
            TractorDive = 4,
            TractorBeam = 5,
            // TractorReturn would be the same as returning (diving is slightly different)
        };

        EnemyComponent(GameObject* pOwner, Enemy enemy, float playerHeight);
        ~EnemyComponent();

        virtual void Update(float deltaTime) override;
        virtual void Render() const override;
        
        void OnLivesUpdated(std::shared_ptr<BaseEvent> e);
        int GetPointsForHit();

        void SetAutonomous(bool autonomous) { m_IsAutomomous = autonomous; };

    private:
        Enemy m_Enemy;
        EnemyState m_State = EnemyState::Formation;

        bool m_IsAutomomous = true;

        // all variables regarding moving
        float m_CurrentTime = 8.0f;

        // diving
        int m_DiveShotCount = 0;

        glm::vec3 m_DesiredLocation{};
        float m_RequiredSpeed = 0.0f;

        // constants (tweak these for better game experience)
        const int m_BreakFormationPercentChance = 20;
        const float m_FormationTime = 12.0f;

        const float m_DiveTime = 3.5f;
        const float m_StationTime = 2.0f;
        const float m_ReturnTime = 2.5f;
        const float m_TractorDive = 3.5f;
        const float m_TractorBeamTime = 4.0f;

        // this value is used to make the above times a bit more random (except formationtime)
        float m_CurrentActionTime = 0.0f;
        const float m_Deviation = 1.0f;

        const int m_MaxDiveShots = 2;
        const float m_PlayerHeight = 0.0f;

        GameObject* m_pTractorBeam = nullptr;

        void DoFormation(float deltaTime);
        void DoDive(float deltaTime);
        void DoStationing(float deltaTime);
        void DoReturn(float deltaTime);
        void DoTractorDive(float deltaTime);
        void DoTractorBeam(float deltaTime);

        float GetRandomDeviationTime();
    };
}
