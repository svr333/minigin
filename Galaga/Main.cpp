#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <Minigin.h>

#include <Services/ResourceManager.h>
#include <Components/TextComponent.h>
#include <Components/FramesComponent.h>
#include <Components/TextureComponent.h>
#include <Components/RotateComponent.h>
#include <Components/HealthComponent.h>
#include <Components/LivesComponent.h>
#include <Components/MenuComponent.h>
#include "Components/ShootComponent.h"
#include "Components/PointsComponent.h"
#include "Components/DisplayLivesComponent.h"
#include "Components/DisplayPointsComponent.h"
#include "Components/HighscoresComponent.h"
#include "Components/LevelComponent.h"

#include <Scenes/Scene.h>
#include <Scenes/SceneManager.h>

#include <Input/InputManager.h>
#include <Commands/MoveCommand.h>
#include <Commands/NextButtonCommand.h>
#include <Commands/TriggerButtonCommand.h>
#include <Commands/SelectSceneCommand.h>
#include "Commands/ShootCommand.h"

#include <Services/ServiceLocator.h>
#include <Events/EventManager.h>
#include "Json/JsonConvert.h"

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
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_RIGHT, "Commands");

	command = new MoveCommand(fps.get(), { -1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_LEFT, "Commands");

	command = new MoveCommand(fps.get(), { 0, 1, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_DOWN, "Commands");

	command = new MoveCommand(fps.get(), { 0, -1, 0 }, speed);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_UP, "Commands");

	scene.Add(fps);
}

void Events()
{
	// tested using debug (not the best but quicker progress)
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Events");
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 40);

	auto player = std::make_shared<GameObject>();
	player->AddComponent(new HealthComponent(player.get(), 0));
	player->AddComponent(new LivesComponent(player.get()));
	player->AddComponent(new TextComponent(player.get(), "PLAYER", font));

	scene.Add(player);
}

void Sounds()
{
	ServiceLocator::GetAudio()->InitializeSound("BackgroundTrack.mp3", Audio::SoundType::BackgroundMusic);
	ServiceLocator::GetAudio()->QueueSound(Audio::Sound{ Audio::SoundType::BackgroundMusic, 8, 1 });
}

void CreateHighscoreScene()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Highscores");

	auto background = std::make_shared<GameObject>();
	background->AddComponent(new TextureComponent(background.get(), "space_background.jpg"));
	scene.Add(background);

	auto highscores = std::make_shared<GameObject>();
	highscores->AddComponent(new HighscoresComponent(highscores.get()));

	//EventManager::GetInstance().QueueEvent({ HighscoreAddedEvent { highscores.get(), "aaa", 5}});
}

void CreateSingleplayer()
{
	// singleplayer scene
	auto& singleplayer = dae::SceneManager::GetInstance().CreateScene("Singleplayer");
	auto& inputManager = dae::InputManager::GetInstance();

	// background
	auto background = std::make_shared<GameObject>();
	background->AddComponent(new TextureComponent(background.get(), "space_background.jpg"));
	singleplayer.Add(background);

	// HUD
	auto player = std::make_shared<GameObject>();
	auto hudFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);

	auto lives = std::make_shared<GameObject>();
	lives->AddComponent(new TextComponent(lives.get(), "Lives: 4", hudFont));
	lives->AddComponent(new DisplayLivesComponent(lives.get(), player));

	lives->GetTransform().SetLocalPosition({ 535, 450, 0 });
	singleplayer.Add(lives);

	auto points = std::make_shared<GameObject>();
	points->AddComponent(new TextComponent(points.get(), "Points: 0", hudFont));
	points->AddComponent(new DisplayPointsComponent(points.get(), player));

	points->GetTransform().SetLocalPosition({ 10, 450, 0 });
	singleplayer.Add(points);

	// player
	player->AddComponent(new TextureComponent(player.get(), "Character.png"));
	player->AddComponent(new ShootComponent(player.get()));

	player->AddComponent(new HealthComponent(player.get()));
	player->AddComponent(new LivesComponent(player.get(), 4));
	player->AddComponent(new PointsComponent(player.get()));

	player->GetTransform().SetScale({ 0.5f, 0.5f, 1 });
	player->GetTransform().SetLocalPosition({ 300, 400, 0 });

	const float speed = 160;

	// Level (enemies) (add to scene first cause reference)
	auto level = std::make_shared<GameObject>();
	singleplayer.Add(level);

	level->AddComponent(new LevelComponent(level.get(), "Enemies3.json", player));

	// controls
	auto cmd = new MoveCommand(player.get(), { 1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(cmd, dae::InputManager::ButtonState::Pressed, SDL_SCANCODE_RIGHT, "Singleplayer");

	cmd = new MoveCommand(player.get(), { -1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(cmd, dae::InputManager::ButtonState::Pressed, SDL_SCANCODE_LEFT, "Singleplayer");

	auto c = new ShootCommand(player.get());
	inputManager.AddKeyboardCommand(c, dae::InputManager::ButtonState::Down, SDL_SCANCODE_SPACE, "Singleplayer");

	singleplayer.Add(player);
}

void CreateCoop()
{
	// singleplayer scene
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Coop");
	auto& inputManager = dae::InputManager::GetInstance();

	// background
	auto background = std::make_shared<GameObject>();
	background->AddComponent(new TextureComponent(background.get(), "space_background.jpg"));
	scene.Add(background);

	// player
	auto player = std::make_shared<GameObject>();
	player->AddComponent(new TextureComponent(player.get(), "Character.png"));
	player->AddComponent(new ShootComponent(player.get()));

	player->AddComponent(new HealthComponent(player.get()));
	player->AddComponent(new LivesComponent(player.get(), 4));
	player->AddComponent(new PointsComponent(player.get()));

	player->GetTransform().SetScale({ 0.5f, 0.5f, 1 });
	player->GetTransform().SetLocalPosition({ 280, 400, 0 });

	scene.Add(player);

	const float speed = 160;

	// controls
	auto cmd = new MoveCommand(player.get(), { 1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(cmd, dae::InputManager::ButtonState::Pressed, SDL_SCANCODE_RIGHT, "Coop");

	cmd = new MoveCommand(player.get(), { -1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(cmd, dae::InputManager::ButtonState::Pressed, SDL_SCANCODE_LEFT, "Coop");

	auto c = new ShootCommand(player.get());
	inputManager.AddKeyboardCommand(c, dae::InputManager::ButtonState::Down, SDL_SCANCODE_SPACE, "Coop");

	// HUD
	auto hudFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 26);

	auto lives = std::make_shared<GameObject>();
	lives->AddComponent(new TextComponent(lives.get(), "Lives: 4", hudFont));
	lives->AddComponent(new DisplayLivesComponent(lives.get(), player));

	lives->GetTransform().SetLocalPosition({ 535, 450, 0 });
	scene.Add(lives);

	auto points = std::make_shared<GameObject>();
	points->AddComponent(new TextComponent(points.get(), "Points: 0", hudFont));
	points->AddComponent(new DisplayPointsComponent(points.get(), player));

	points->GetTransform().SetLocalPosition({ 10, 450, 0 });
	scene.Add(points);

	// player 2
	auto player2 = std::make_shared<GameObject>();
	player2->AddComponent(new TextureComponent(player2.get(), "Character.png"));
	player2->AddComponent(new ShootComponent(player2.get()));

	player2->AddComponent(new HealthComponent(player2.get()));
	player2->AddComponent(new LivesComponent(player2.get(), 4));
	player2->AddComponent(new PointsComponent(player2.get()));

	player2->GetTransform().SetScale({ 0.5f, 0.5f, 1 });
	player2->GetTransform().SetLocalPosition({ 330, 400, 0 });

	// controls
	cmd = new MoveCommand(player2.get(), { 1, 0, 0 }, speed);
	inputManager.AddControllerCommand(cmd, 0, dae::InputManager::ButtonState::Pressed, dae::Xbox360Controller::ControllerButton::DPadRight, "Coop");

	cmd = new MoveCommand(player2.get(), { 1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(cmd, dae::InputManager::ButtonState::Pressed, SDL_SCANCODE_D, "Coop");

	cmd = new MoveCommand(player2.get(), { -1, 0, 0 }, speed);
	inputManager.AddControllerCommand(cmd, 0, dae::InputManager::ButtonState::Pressed, dae::Xbox360Controller::ControllerButton::DPadLeft, "Coop");

	cmd = new MoveCommand(player2.get(), { -1, 0, 0 }, speed);
	inputManager.AddKeyboardCommand(cmd, dae::InputManager::ButtonState::Pressed, SDL_SCANCODE_A, "Coop");

	auto shoot = new ShootCommand(player2.get());
	inputManager.AddControllerCommand(shoot, 0, dae::InputManager::ButtonState::Pressed, dae::Xbox360Controller::ControllerButton::ButtonA, "Coop");

	shoot = new ShootCommand(player2.get());
	inputManager.AddKeyboardCommand(shoot, dae::InputManager::ButtonState::Down, SDL_SCANCODE_F, "Coop");

	// HUD
	auto lives2 = std::make_shared<GameObject>();
	lives2->AddComponent(new TextComponent(lives2.get(), "Lives: 4", hudFont, { 0, 255, 255 }));
	lives2->AddComponent(new DisplayLivesComponent(lives2.get(), player2));

	lives2->GetTransform().SetLocalPosition({ 535, 420, 0 });
	scene.Add(lives2);

	auto points2 = std::make_shared<GameObject>();
	points2->AddComponent(new TextComponent(points2.get(), "Points: 0", hudFont, { 0, 255, 255 }));
	points2->AddComponent(new DisplayPointsComponent(points2.get(), player2));

	points2->GetTransform().SetLocalPosition({ 10, 420, 0 });
	scene.Add(points2);

	scene.Add(player2);

	// Level (enemies) (add to scene first cause reference)
	auto level = std::make_shared<GameObject>();
	scene.Add(level);

	level->AddComponent(new LevelComponent(level.get(), "Enemies3.json", player));
}

void CreateVersus()
{
	// test
	auto& newScene = dae::SceneManager::GetInstance().CreateScene("Versus");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);

	auto test = std::make_shared<GameObject>();
	test->AddComponent(new TextComponent(test.get(), "Test", font));
	newScene.Add(test);
}
void CreateMenu()
{
	// initialize audio
	ServiceLocator::GetAudio()->InitializeSound("BackgroundTrack.mp3", Audio::SoundType::BackgroundMusic);
	ServiceLocator::GetAudio()->InitializeSound("ShootBullet.mp3", Audio::SoundType::ShootBullet);
	//ServiceLocator::GetAudio()->QueueSound(Audio::Sound{ Audio::SoundType::BackgroundMusic, 8, 1 });

	auto& scene = dae::SceneManager::GetInstance().CreateScene("Menu");
	auto& inputManager = dae::InputManager::GetInstance();

	// background
	auto background = std::make_shared<GameObject>();
	background->AddComponent(new TextureComponent(background.get(), "space_background.jpg"));
	scene.Add(background);

	// menu
	auto menuObj = std::make_shared<GameObject>();
	auto menu = new MenuComponent(menuObj.get());

	menuObj->AddComponent(menu);
	scene.Add(menuObj);

	// menu buttons
	auto menuFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 32);

	auto single = menu->AddButton("Singleplayer", menuFont,
		{ 255, 255, 255 }, { 0, 255, 255 }, 230, 150, new SelectSceneCommand("Singleplayer"));
	scene.Add(single);

	auto coop = menu->AddButton("Co-op", menuFont,
		{ 255, 255, 255 }, { 0, 255, 255 }, 230, 225, new SelectSceneCommand("Coop"));
	scene.Add(coop);

	auto versus = menu->AddButton("Versus", menuFont,
		{ 255, 255, 255 }, { 0, 255, 255 }, 230, 300, new SelectSceneCommand("Versus"));
	scene.Add(versus);

	// controls for selecting button
	auto command = new NextButtonCommand(menuObj.get(), false);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_DOWN, "Menu");

	command = new NextButtonCommand(menuObj.get(), true);
	inputManager.AddKeyboardCommand(command, dae::InputManager::ButtonState::Down, SDL_SCANCODE_UP, "Menu");

	auto triggerCommand = new TriggerButtonCommand(menuObj.get());
	inputManager.AddKeyboardCommand(triggerCommand, dae::InputManager::ButtonState::Down, SDL_SCANCODE_RETURN, "Menu");

	CreateSingleplayer();
	CreateCoop();
	CreateVersus();

	CreateHighscoreScene();
}

void TestScores()
{
	auto read = JsonConvert::GetInstance().DeserializeHighscores();
	std::vector<Highscore> scores = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 }, { "E", 5 }};

	JsonConvert::GetInstance().SerializeHighscores(scores);

	read = JsonConvert::GetInstance().DeserializeHighscores();
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(CreateMenu);
    return 0;
}
 