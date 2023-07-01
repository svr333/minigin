#pragma once
#include "Command.h"
#include <glm/glm.hpp>

namespace dae
{
	class GameObject;

	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject* obj, glm::vec3 direction = {}, float speed = 0.0f);
		void Execute(float deltaTime) override;

		void SetMovement(glm::vec3 direction, float speed) { m_Direction = direction, m_Speed = speed; };

	private:
		GameObject* m_pObject = nullptr;
		glm::vec3 m_Direction{};
		float m_Speed = 0.0f;
	};

}
