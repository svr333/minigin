#pragma once
#include "BaseComponent.h"
#include <glm/glm.hpp>

namespace dae
{
	class MovementComponent : public BaseComponent
	{
	public:
		MovementComponent(GameObject* pOwner, const glm::vec3& direction, float speed = 50.0f, float maxDistance = 400.0f);

		virtual void Update(float deltaTime) override;
		virtual void Render() const override;

		bool IsFinished();

	private:
		float m_Speed = 10.0f;
		glm::vec3 m_Direction {};

		float m_CurrentDistance = 0.0f;
		const float m_MaxDistance = 500.0f;

		glm::vec3 m_StartPos;
	};
}
