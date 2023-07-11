#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class TriggerButtonCommand : public Command
	{
	public:
		TriggerButtonCommand(GameObject* pObject);

		virtual void Execute(float deltaTime) override;

	private:
		GameObject* m_pObject;
	};
}
