#pragma once
#include "Command.h"
#include <string>

namespace dae
{
	class SelectSceneCommand : public Command
	{
	public:
		SelectSceneCommand(const std::string& sceneName);
		virtual void Execute(float deltaTime) override;

	private:
		std::string m_SceneName;
	};
}
