#include "SelectSceneCommand.h"
#include "../Scenes/SceneManager.h"

dae::SelectSceneCommand::SelectSceneCommand(const std::string& sceneName)
	: Command()
	, m_SceneName(sceneName)
{
}

void dae::SelectSceneCommand::Execute(float /*deltaTime*/)
{
	SceneManager::GetInstance().SetActiveScene(m_SceneName);
}
