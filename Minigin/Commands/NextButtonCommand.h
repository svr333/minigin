#pragma once
#include "Command.h"

namespace dae
{
	class GameObject;

	class NextButtonCommand : public Command
	{
	public:
		NextButtonCommand(GameObject* obj, bool previous = false);

		virtual void Execute(float deltaTime) override;

	private:
		GameObject* m_pObject;
		bool m_Previous;
	};
}
