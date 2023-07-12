#pragma once
#include <Commands/Command.h>

namespace dae
{
	class GameObject;

	class ShootCommand : public Command
	{
	public:
		ShootCommand(GameObject* pObject);

		virtual void Execute(float deltaTime) override;

	private:
		GameObject* m_pObject;
	};
}
