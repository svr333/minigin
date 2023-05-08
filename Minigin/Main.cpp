#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextComponent.h"
#include "FramesComponent.h"
#include "TextureComponent.h"
#include "Scene.h"

using namespace dae;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// background
	auto go = std::make_shared<GameObject>();
	go->AddComponent(new TextureComponent(go.get(), "background.tga"));
	scene.Add(go);

	// logo
	go = std::make_shared<GameObject>();
	go->AddComponent(new TextureComponent(go.get(), "logo.tga"));
	go->GetTransform().SetPosition({216, 180, 0});
	scene.Add(go);

	// text
	auto to = std::make_shared<GameObject>();
	to->GetTransform().SetPosition({ 80, 20, 0 });

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	to->AddComponent(new TextComponent(to.get(), "Programming 4 Assignment", font));
	scene.Add(to);

	// fps counter
	auto fps = std::make_shared<GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	fps->AddComponent(new FramesComponent(fps.get()));
	fps->AddComponent(new TextComponent(fps.get(), "FPS", font));

	scene.Add(fps);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}