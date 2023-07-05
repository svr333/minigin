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
#include "RotateComponent.h"
#include "HealthComponent.h"
#include "LivesComponent.h"
#include "Scene.h"

#include "InputManager.h"
#include "MoveCommand.h"

using namespace dae;

void Rotator()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// background
	auto go = std::make_shared<GameObject>();
	go->AddComponent(new TextureComponent(go.get(), "background.tga"));
	scene.Add(go);

	// logo
	go = std::make_shared<GameObject>();
	go->AddComponent(new TextureComponent(go.get(), "logo.tga"));
	go->GetTransform().SetLocalPosition({ 216, 180, 0 });
	scene.Add(go);

	// text
	auto to = std::make_shared<GameObject>();
	to->GetTransform().SetLocalPosition({ 80, 20, 0 });

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	to->AddComponent(new TextComponent(to.get(), "Programming 4 Assignment", font));
	scene.Add(to);

	// fps counter
	auto fps = std::make_shared<GameObject>();
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 16);

	fps->AddComponent(new FramesComponent(fps.get()));
	fps->AddComponent(new TextComponent(fps.get(), "FPS", font));

	scene.Add(fps);

	// 'character'
	auto ch = std::make_shared<GameObject>();
	ch->GetTransform().SetLocalPosition({ 280, 300, 0 });
	font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);

	ch->AddComponent(new TextComponent(ch.get(), "A", font));
	ch->AddComponent(new RotateComponent(ch.get(), 50));

	auto ch2 = std::make_shared<GameObject>();
	ch2->SetParent(ch.get());

	ch2->AddComponent(new TextComponent(ch2.get(), "B", font));
	ch2->GetTransform().SetLocalPosition({ 0, 0, 0 });
	ch2->AddComponent(new RotateComponent(ch2.get(), 30, 200, false));

	scene.Add(ch);
	scene.Add(ch2);
}

void Commands()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Commands");
	auto& inputManager = dae::InputManager::GetInstance();

	auto fps = std::make_shared<GameObject>();
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);

	fps->GetTransform().SetLocalPosition({ 216, 180, 0 });
	fps->AddComponent(new FramesComponent(fps.get()));
	fps->AddComponent(new TextComponent(fps.get(), "FPS", font));

	float speed = 200;

	auto command = new MoveCommand(fps.get(), { 1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_RIGHT);

	command = new MoveCommand(fps.get(), { -1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_LEFT);

	command = new MoveCommand(fps.get(), { 0, 1, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_DOWN);

	command = new MoveCommand(fps.get(), { 0, -1, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_UP);

	scene.Add(fps);
}

void Events()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Events");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);

	auto player = std::make_shared<GameObject>();
	player->AddComponent(new HealthComponent(player.get(), 0));
	player->AddComponent(new LivesComponent(player.get()));
	player->AddComponent(new TextComponent(player.get(), "PLAYER", font));

	scene.Add(player);
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(Events);
    return 0;
}
 