#pragma once
#include <Components/BaseComponent.h>

namespace dae
{
    class EnemyComponent : public BaseComponent
    {
    public:
        EnemyComponent(GameObject* pOwner);

        virtual void Update(float deltaTime) override;
        virtual void Render() const override;
    };
}
