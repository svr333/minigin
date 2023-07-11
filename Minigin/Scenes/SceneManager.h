#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../Services/Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void Render();

		void SetActiveScene(const std::string& sceneName);
		void SetActiveScene(int sceneId);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;

		std::vector<std::shared_ptr<Scene>> m_Scenes;
		int m_ActiveSceneId = 0;
	};
}
