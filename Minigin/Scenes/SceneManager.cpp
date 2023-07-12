#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	// we can update even inactive scenes but doesnt seem useful for us
	if (m_Scenes.size() > 0)
	{
		m_Scenes[m_ActiveSceneId]->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	if (m_Scenes.size() > 0)
	{
		m_Scenes[m_ActiveSceneId]->Render();
	}
}

void dae::SceneManager::SetActiveScene(const std::string& sceneName)
{
	for (int i = 0; i < (int)m_Scenes.size(); i++)
	{
		if (m_Scenes[i]->GetName() == sceneName)
		{
			SetActiveScene(i);
			return;
		}
	}
}

void dae::SceneManager::SetActiveScene(int sceneId)
{
	if (sceneId >= (int)m_Scenes.size())
	{
		return;
	}

	m_ActiveSceneId = sceneId;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}
