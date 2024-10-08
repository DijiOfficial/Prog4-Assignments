#include "GameLoader.h"

#include "ResourceManager.h"
#include "SceneManager.h"
#include "Components.h"
#include "PickUpLoader.h"
#include "InputManager.h"
#include "ISoundSystem.h"
#include "Collision.h"
#include "CustomCommands.h"
#include "FPSCounter.h"
#include "ScoreCounter.h"
#include "HealthCounter.h"
#include "AI.h"
//#include "PinkAI.h"
#include "GhostAI.h"
#include "Observers.h"
#include "GhostCollision.h"
#include "GhostsAlgorithm.h"
#include "GameState.h"
#include "LevelIntro.h"
#include "PelletCounter.h"
#include "Menu.h"
#include "EnterName.h"
#include "CustomTextRender.h"
#include "ScoreBoard.h"
#include "Fruit.h"
#include <format>
#include "BlinkingText.h"
#include "PickUp.h"
#include "Intro.h"
using namespace diji;

namespace loader
{
	constexpr glm::vec2 VIEWPORT{ 452, 608 };
}

void Loader::PacmanMenuReload(bool isFirstLoad)
{
	//todo: const correctness the CreateGameObjects
	const auto& introScene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::INTRO));
#pragma region Animation
	const auto& characterNickname = introScene->GetGameObject("characterNickname");

	if (!isFirstLoad)
	{
		const auto& player = introScene->GetGameObject("player");
		const auto& powerPellet = introScene->GetGameObject("powerPelletTexture");
		const auto& powerUp = introScene->GetGameObject("powerPellet");
		const auto& GhostTimers = introScene->GetGameObject("ghostTimers");
		const auto& Blinky = introScene->GetGameObject("z_Blinky");
		const auto& Pinky = introScene->GetGameObject("z_Pinky");
		const auto& Inky = introScene->GetGameObject("z_Inky");
		const auto& Clyde = introScene->GetGameObject("z_Clyde");

		powerUp->GetComponent<pacman::PickUp>()->RemoveObserver(static_cast<diji::MessageTypes>(pacman::MessageTypesDerived::POWERUP_COLLISION), player->GetComponent<pacman::AI>());

		characterNickname->GetComponent<pacman::IntroRender>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), player->GetComponent<pacman::AI>());
		characterNickname->GetComponent<pacman::IntroRender>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), GhostTimers->GetComponent<pacman::GhostsTimers>());

		characterNickname->GetComponent<pacman::IntroRender>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_BEGIN_TWO), powerUp->GetComponent<pacman::PickUp>());
		characterNickname->GetComponent<pacman::IntroRender>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), powerUp->GetComponent<pacman::PickUp>());
		characterNickname->GetComponent<pacman::IntroRender>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_BEGIN), powerPellet->GetComponent<pacman::PickUp>());
		characterNickname->GetComponent<pacman::IntroRender>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), powerPellet->GetComponent<pacman::PickUp>());
		
		Clyde->GetComponent<pacman::Clyde>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::RESET_MENU), characterNickname->GetComponent<pacman::Intro>());
		Clyde->GetComponent<pacman::Clyde>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::RESET_MENU), powerUp->GetComponent<pacman::PickUp>());
		Clyde->GetComponent<pacman::Clyde>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::RESET_MENU), powerPellet->GetComponent<pacman::PickUp>());
		
		const std::vector<GameObject*> ghosts = { Blinky, Pinky, Inky, Clyde };
		for (const auto& ghost : ghosts)
		{
			powerUp->GetComponent<pacman::PickUp>()->RemoveObserver(static_cast<diji::MessageTypes>(pacman::MessageTypesDerived::POWERUP_COLLISION), ghost->GetComponent<pacman::GhostAI>());
		}

		Blinky->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
		Blinky->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Blinky->GetComponent<pacman::RedAI>());
		Pinky->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
		Pinky->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Pinky->GetComponent<pacman::Pinky>());
		Inky->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
		Inky->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Inky->GetComponent<pacman::Inky>());
		Clyde->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
		Clyde->GetComponent<pacman::GhostCollision>()->RemoveObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Clyde->GetComponent<pacman::Clyde>());

		introScene->Remove("player");
		introScene->Remove("powerPelletTexture");
		introScene->Remove("powerPellet");
		introScene->Remove("ghostTimers");
		introScene->Remove("pelletCounter");
		introScene->Remove("z_Blinky");
		introScene->Remove("z_Pinky");
		introScene->Remove("z_Inky");
		introScene->Remove("z_Clyde");
	}

	const auto& player = introScene->CreateGameObject("player");
	const auto& powerPellet = introScene->CreateGameObject("powerPelletTexture");
	const auto& powerUp = introScene->CreateGameObject("powerPellet");
	const auto& GhostTimers = introScene->CreateGameObject("ghostTimers");
	const auto& pelletCounter = introScene->CreateGameObject("pelletCounter");
	const auto& Blinky = introScene->CreateGameObject("z_Blinky");
	const auto& Pinky = introScene->CreateGameObject("z_Pinky");
	const auto& Inky = introScene->CreateGameObject("z_Inky");
	const auto& Clyde = introScene->CreateGameObject("z_Clyde");


	player->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player->GetComponent<Texture>()->SetRotation(true);
	player->AddComponents<Transform>(452, 332);
	player->AddComponents<Render>(2);
	player->AddComponents<pacman::HealthCounter>(4);
	player->AddComponents<pacman::ScoreCounter>(0);
	player->AddComponents<Collider>(15, 15);
	player->AddComponents<pacman::AI>();
	player->GetComponent<Texture>()->SetRotationAngle(static_cast<int>(diji::Movement::Left) * 90.0f);

	powerPellet->AddComponents<diji::Texture>("PowerPellet.png");
	powerPellet->AddComponents<diji::Transform>(loader::VIEWPORT.x * 0.35f + 4.f, 440.f);
	powerPellet->AddComponents<diji::Render>(2);
	powerPellet->AddComponents<diji::Collider>(8, 8);
	powerPellet->AddComponents<pacman::PickUp>(player, nullptr, -1);
	powerPellet->GetComponent<Render>()->DisableRender();
	powerPellet->GetComponent<pacman::PickUp>()->DisablePickUp();

	powerUp->AddComponents<diji::Texture>("PowerPellet.png", 8, 8);
	powerUp->AddComponents<diji::Transform>(70, 340);
	powerUp->AddComponents<diji::Render>(2);
	powerUp->AddComponents<diji::Collider>(8, 8);
	powerUp->AddComponents<pacman::PickUp>(player, nullptr, -1);
	powerUp->GetComponent<Render>()->DisableRender();
	powerUp->GetComponent<pacman::PickUp>()->DisablePickUp();
	powerUp->GetComponent<pacman::PickUp>()->AddObserver(static_cast<diji::MessageTypes>(pacman::MessageTypesDerived::POWERUP_COLLISION), player->GetComponent<pacman::AI>());

	GhostTimers->AddComponents<pacman::GhostsTimers>();

	pelletCounter->AddComponents<pacman::PelletObserver>();
	pelletCounter->AddComponents<pacman::PelletCounter>();

	Blinky->AddComponents<Texture>("RedGhost.png", 15, 15, 2);
	Blinky->AddComponents<Transform>(495, 332);
	Blinky->AddComponents<Render>(2);
	Blinky->AddComponents<Collider>(15, 15);
	Blinky->AddComponents<pacman::RedAI>(player, pelletCounter, GhostTimers);
	Blinky->AddComponents<pacman::GhostCollision>(player);
	Blinky->GetComponent<pacman::RedAI>()->SetInMenu();

	Pinky->AddComponents<Texture>("Pinky.png", 15, 15, 2);
	Pinky->AddComponents<Transform>(529, 332);
	Pinky->AddComponents<Render>(2);
	Pinky->AddComponents<Collider>(15, 15);
	Pinky->AddComponents<pacman::Pinky>(player, pelletCounter, GhostTimers);
	Pinky->AddComponents<pacman::GhostCollision>(player);
	Pinky->GetComponent<pacman::Pinky>()->SetInMenu();

	Inky->AddComponents<Texture>("Inky.png", 15, 15, 2);
	Inky->AddComponents<Transform>(563, 332);
	Inky->AddComponents<Render>(2);
	Inky->AddComponents<Collider>(15, 15);
	Inky->AddComponents<pacman::Inky>(player, pelletCounter, GhostTimers, Blinky);
	Inky->AddComponents<pacman::GhostCollision>(player);
	Inky->GetComponent<pacman::Inky>()->SetInMenu();

	Clyde->AddComponents<Texture>("Clyde.png", 15, 15, 2);
	Clyde->AddComponents<Transform>(597, 332);
	Clyde->AddComponents<Render>(2);
	Clyde->AddComponents<Collider>(15, 15);
	Clyde->AddComponents<pacman::Clyde>(player, pelletCounter, GhostTimers);
	Clyde->AddComponents<pacman::GhostCollision>(player);
	Clyde->GetComponent<pacman::Clyde>()->SetInMenu();

	////Set the ghosts aware of each other
	const auto& BlinkyAI = Blinky->GetComponent<pacman::RedAI>();
	const auto& PinkyAI = Pinky->GetComponent<pacman::Pinky>();
	const auto& InkyAI = Inky->GetComponent<pacman::Inky>();
	const auto& ClydeAI = Clyde->GetComponent<pacman::Clyde>();
	const std::vector<pacman::GhostAI*> ghostAIs = { BlinkyAI, PinkyAI, InkyAI, ClydeAI };
	BlinkyAI->SetGhostsVector(ghostAIs);
	PinkyAI->SetGhostsVector(ghostAIs);
	InkyAI->SetGhostsVector(ghostAIs);
	ClydeAI->SetGhostsVector(ghostAIs);

	const std::vector<GameObject*> ghosts = { Blinky, Pinky, Inky, Clyde };
	for (const auto& ghost : ghosts)
	{
		powerUp->GetComponent<pacman::PickUp>()->AddObserver(static_cast<diji::MessageTypes>(pacman::MessageTypesDerived::POWERUP_COLLISION), ghost->GetComponent<pacman::GhostAI>());
	}

	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Blinky->GetComponent<pacman::RedAI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Pinky->GetComponent<pacman::Pinky>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Inky->GetComponent<pacman::Inky>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Clyde->GetComponent<pacman::Clyde>());
#pragma endregion
	
	characterNickname->GetComponent<pacman::IntroRender>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), player->GetComponent<pacman::AI>());
	characterNickname->GetComponent<pacman::IntroRender>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), GhostTimers->GetComponent<pacman::GhostsTimers>());

	characterNickname->GetComponent<pacman::IntroRender>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_BEGIN_TWO), powerUp->GetComponent<pacman::PickUp>());
	characterNickname->GetComponent<pacman::IntroRender>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), powerUp->GetComponent<pacman::PickUp>());
	characterNickname->GetComponent<pacman::IntroRender>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_BEGIN), powerPellet->GetComponent<pacman::PickUp>());
	characterNickname->GetComponent<pacman::IntroRender>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::MENU_ANIMATION_BEGIN), powerPellet->GetComponent<pacman::PickUp>());

	Clyde->GetComponent<pacman::Clyde>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::RESET_MENU), powerUp->GetComponent<pacman::PickUp>());
	Clyde->GetComponent<pacman::Clyde>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::RESET_MENU), powerPellet->GetComponent<pacman::PickUp>());
	Clyde->GetComponent<pacman::Clyde>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::RESET_MENU), characterNickname->GetComponent<pacman::Intro>());

	if (!isFirstLoad)
	{
		player->Init();
		powerPellet->Init();
		powerUp->Init();
		GhostTimers->Init();
		pelletCounter->Init();
		Blinky->Init();
		Pinky->Init();
		Inky->Init();
		Clyde->Init();
	}
}

void Loader::PacmanIntro()
{
	const auto& introScene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::INTRO));
	const auto& textFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	auto HUD = introScene->CreateGameObject("HUD");
	HUD->AddComponents<Transform>(10, 10);

	auto scoreCounter = introScene->CreateGameObject("scoreCounterHUD");
	scoreCounter->AddComponents<Transform>(-5, 20);
	scoreCounter->AddComponents<pacman::ScoreObserver>("     00", textFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->GetComponent<Render>()->DisableRender();
	scoreCounter->SetParent(HUD, false);

	auto playerText = introScene->CreateGameObject("playerTextHUD");
	playerText->AddComponents<Transform>(65, 0);
	playerText->AddComponents<Text>("1UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	playerText->AddComponents<Render>();
	playerText->GetComponent<Render>()->DisableRender();
	playerText->SetParent(HUD, false);

	auto twoPlayerText = introScene->CreateGameObject("twoPlayerTextHUD");
	twoPlayerText->AddComponents<Transform>(370, 0);
	twoPlayerText->AddComponents<Text>("2UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	twoPlayerText->AddComponents<Render>();
	twoPlayerText->GetComponent<Render>()->DisableRender();
	twoPlayerText->SetParent(HUD, false);

	auto highScoreText = introScene->CreateGameObject("highScoreTextHUD");
	highScoreText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 10, 0.f);
	highScoreText->AddComponents<Text>("HIGH SCORE", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	highScoreText->AddComponents<Render>();
	highScoreText->GetComponent<Render>()->DisableRender();
	highScoreText->SetParent(HUD, false);

	auto highScoreDisplay = introScene->CreateGameObject("highScoreDisplayHUD");
	highScoreDisplay->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 75.f, 20.f);
	highScoreDisplay->AddComponents<pacman::HighScoreObserver>("  ", textFont, SDL_Color{ 255, 255, 255, 255 }, false);
	highScoreDisplay->AddComponents<Render>();
	highScoreDisplay->GetComponent<Render>()->DisableRender();
	highScoreDisplay->SetParent(HUD, false);

	auto characterNicknameTitle = introScene->CreateGameObject("characterNicknameTitle");
	characterNicknameTitle->AddComponents<Transform>(loader::VIEWPORT.x * 0.50f, 80.f);
	characterNicknameTitle->AddComponents<Text>("CHARACTER / NICKNAME", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	characterNicknameTitle->AddComponents<Render>();
	characterNicknameTitle->GetComponent<Render>()->DisableRender();
	characterNicknameTitle->SetParent(HUD, false);

	auto credit = introScene->CreateGameObject("creditHUD");
	credit->AddComponents<Transform>(100.f, loader::VIEWPORT.y - 50);
	credit->AddComponents<Text>("credit  1", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	credit->AddComponents<Render>();
	credit->GetComponent<Render>()->DisableRender();
	credit->SetParent(HUD, false);

#pragma region ghostsTextures
	auto redGhostTexture = introScene->CreateGameObject("RedGhostTexture");
	redGhostTexture->AddComponents<Texture>("RedGhost.png", 15, 15, 1);
	redGhostTexture->AddComponents<Transform>(30.f, 115.f);
	redGhostTexture->AddComponents<Render>(2);
	redGhostTexture->GetComponent<Render>()->DisableRender();
	redGhostTexture->GetComponent<Texture>()->PauseAnimation();
	redGhostTexture->SetParent(HUD, false);

	auto PinkyTexture = introScene->CreateGameObject("PinkyTexture");
	PinkyTexture->AddComponents<Texture>("Pinky.png", 15, 15, 1);
	PinkyTexture->AddComponents<Transform>(30.f, 165.f);
	PinkyTexture->AddComponents<Render>(2);
	PinkyTexture->GetComponent<Render>()->DisableRender();
	PinkyTexture->GetComponent<Texture>()->PauseAnimation();
	PinkyTexture->SetParent(HUD, false);

	auto InkyTexture = introScene->CreateGameObject("InkyTexture");
	InkyTexture->AddComponents<Texture>("Inky.png", 15, 15, 1);
	InkyTexture->AddComponents<Transform>(30.f, 215.f);
	InkyTexture->AddComponents<Render>(2);
	InkyTexture->GetComponent<Render>()->DisableRender();
	InkyTexture->GetComponent<Texture>()->PauseAnimation();
	InkyTexture->SetParent(HUD, false);

	auto ClydeTexture = introScene->CreateGameObject("ClydeTexture");
	ClydeTexture->AddComponents<Texture>("Clyde.png", 15, 15, 1);
	ClydeTexture->AddComponents<Transform>(30.f, 265.f);
	ClydeTexture->AddComponents<Render>(2);
	ClydeTexture->GetComponent<Render>()->DisableRender();
	ClydeTexture->GetComponent<Texture>()->PauseAnimation();
	ClydeTexture->SetParent(HUD, false);

	auto pellet = introScene->CreateGameObject("pelletTexture");
	pellet->AddComponents<Texture>("bruh.png");
	pellet->AddComponents<Transform>(loader::VIEWPORT.x * 0.35f, 400.f);
	pellet->AddComponents<Render>(2);
	pellet->GetComponent<Render>()->DisableRender();
	pellet->SetParent(HUD, false);
#pragma endregion
	auto brandName = introScene->CreateGameObject("brandNameHUD");
	brandName->AddComponents<Transform>(loader::VIEWPORT.x * 0.50f, loader::VIEWPORT.y * 0.82f);
	brandName->AddComponents<Text>("Burgisser Dylan", textFont, SDL_Color{ 255, 184, 222, 255 }, true);
	brandName->AddComponents<Render>();
	brandName->GetComponent<Render>()->DisableRender();
	brandName->SetParent(HUD, false);

	std::vector<Render*> ghostTextures = { redGhostTexture->GetComponent<Render>(), PinkyTexture->GetComponent<Render>(), InkyTexture->GetComponent<Render>(), ClydeTexture->GetComponent<Render>(), pellet->GetComponent<Render>(), brandName->GetComponent<Render>() };
	std::vector<Render*> textsRenderer = { scoreCounter->GetComponent<Render>(), playerText->GetComponent<Render>(), twoPlayerText->GetComponent<Render>(), highScoreText->GetComponent<Render>(), highScoreDisplay->GetComponent<Render>(), characterNicknameTitle->GetComponent<Render>(), credit->GetComponent<Render>() };
	auto characterNickname = introScene->CreateGameObject("characterNickname");
	characterNickname->AddComponents<Transform>(85.f, 120.f);
	characterNickname->AddComponents<pacman::Intro>();
	characterNickname->AddComponents<pacman::IntroRender>();
	characterNickname->GetComponent<pacman::IntroRender>()->AddGhostRender(std::move(ghostTextures));
	characterNickname->GetComponent<pacman::IntroRender>()->AddTextsRender(std::move(textsRenderer));
	characterNickname->SetParent(HUD, false);

	PacmanMenuReload(true);

	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::SingleCommands>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::Start, HUD, pacman::SingleCommands::SingleCommandOption::SKIP_INTRO);
	input.BindCommand<pacman::SingleCommands>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::A, HUD, pacman::SingleCommands::SingleCommandOption::SKIP_INTRO);

	input.BindCommand<pacman::SingleCommands>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_RETURN, HUD, pacman::SingleCommands::SingleCommandOption::SKIP_INTRO);
	input.BindCommand<pacman::SingleCommands>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_ESCAPE, HUD, pacman::SingleCommands::SingleCommandOption::SKIP_INTRO);
}

void Loader::PacmanMenu()
{
	const auto& menuScene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::MENU));
	const auto& mediumFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);
	const auto& textFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	auto menuUI = menuScene->CreateGameObject("menuUI"); //will handle the drawings and AI part of the menu
	menuUI->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, loader::VIEWPORT.y * 0.5f + 40.f);
	menuUI->AddComponents<pacman::Menu>();
	menuUI->AddComponents<pacman::MenuRender>();
#pragma region HUD
	auto scoreCounter = menuScene->CreateGameObject("scoreCounterHUD");
	scoreCounter->AddComponents<Transform>(5, 30);
	scoreCounter->AddComponents<pacman::ScoreObserver>("     00", textFont);
	scoreCounter->AddComponents<Render>();

	auto playerTextHUD = menuScene->CreateGameObject("playerTextHUD");
	playerTextHUD->AddComponents<Transform>(75, 10);
	playerTextHUD->AddComponents<Text>("1UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	playerTextHUD->AddComponents<Render>();

	auto twoPlayerTextHUD = menuScene->CreateGameObject("twoPlayerTextHUD");
	twoPlayerTextHUD->AddComponents<Transform>(380, 10);
	twoPlayerTextHUD->AddComponents<Text>("2UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	twoPlayerTextHUD->AddComponents<Render>();

	auto highScoreText = menuScene->CreateGameObject("highScoreTextHUD");
	highScoreText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 10.f);
	highScoreText->AddComponents<Text>("HIGH SCORE", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	highScoreText->AddComponents<Render>();

	auto highScoreDisplay = menuScene->CreateGameObject("highScoreDisplayHUD");
	highScoreDisplay->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 65.f, 30.f);
	highScoreDisplay->AddComponents<pacman::HighScoreObserver>("  ", textFont, SDL_Color{ 255, 255, 255, 255 }, false);
	highScoreDisplay->AddComponents<Render>();
#pragma endregion

	auto backText = menuScene->CreateGameObject("backText");
	backText->AddComponents<Transform>(-108, 130);
	backText->AddComponents<Text>("BACK", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	backText->AddComponents<Render>();
	backText->SetParent(menuUI, false);
	backText->GetComponent<Render>()->DisableRender();

#pragma region multiplayer
	auto coopText = menuScene->CreateGameObject("coopText");
	coopText->AddComponents<Transform>(-108, -120);
	coopText->AddComponents<Text>("COOP", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	coopText->AddComponents<Render>();
	coopText->SetParent(menuUI, false);
	coopText->GetComponent<Render>()->DisableRender();

	auto versusText = menuScene->CreateGameObject("versusText");
	versusText->AddComponents<Transform>(-108, -70);
	versusText->AddComponents<Text>("VERSUS", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	versusText->AddComponents<Render>();
	versusText->SetParent(menuUI, false);
	versusText->GetComponent<Render>()->DisableRender();

#pragma region multiplayerSelection
	auto freeMode = menuScene->CreateGameObject("freeMode");
	freeMode->AddComponents<Transform>(-108, -120);
	freeMode->AddComponents<Text>("FREEMODE", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	freeMode->AddComponents<Render>();
	freeMode->SetParent(menuUI, false);
	freeMode->GetComponent<Render>()->DisableRender();

	auto ghostRules = menuScene->CreateGameObject("ghostRules");
	ghostRules->AddComponents<Transform>(-108, -70);
	ghostRules->AddComponents<Text>("GHOST RULES", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	ghostRules->AddComponents<Render>();
	ghostRules->SetParent(menuUI, false);
	ghostRules->GetComponent<Render>()->DisableRender();

#pragma endregion
#pragma endregion

	auto playerText = menuScene->CreateGameObject("singlePlayerText");
	playerText->AddComponents<Transform>(-108, -120);
	playerText->AddComponents<Text>("1 PLAYER", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	playerText->AddComponents<Render>();
	playerText->SetParent(menuUI, false);

	auto twoPlayerText = menuScene->CreateGameObject("twoPlayerText");
	twoPlayerText->AddComponents<Transform>(-108, -70);
	twoPlayerText->AddComponents<Text>("2 PLAYERS", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	twoPlayerText->AddComponents<Render>();
	twoPlayerText->SetParent(menuUI, false);

	auto createLevel = menuScene->CreateGameObject("levelCreator");
	createLevel->AddComponents<Transform>(0, -20);
	createLevel->AddComponents<Text>("CREATE LEVEL", mediumFont, SDL_Color{ 244, 244, 244, 255 }, true);
	createLevel->AddComponents<Render>();
	createLevel->SetParent(menuUI, false);

	//todo: in options add starting lives, addition life, sound ect...
	auto options = menuScene->CreateGameObject("optionsHUD");
	options->AddComponents<Transform>(-108, 30);
	options->AddComponents<Text>("OPTIONS", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	options->AddComponents<Render>();
	options->SetParent(menuUI, false);

	auto highScores = menuScene->CreateGameObject("highScoresHUD");
	highScores->AddComponents<Transform>(-108, 80);
	highScores->AddComponents<Text>("HIGHSCORES", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	highScores->AddComponents<Render>();
	highScores->SetParent(menuUI, false);

	auto exitText = menuScene->CreateGameObject("exitText");
	exitText->AddComponents<Transform>(-108, 130);
	exitText->AddComponents<Text>("EXIT", mediumFont, SDL_Color{ 244, 244, 244, 255 }, false);
	exitText->AddComponents<Render>();
	exitText->SetParent(menuUI, false);

	auto logo = menuScene->CreateGameObject("logo");
	logo->AddComponents<Texture>("Logo.png");
	logo->AddComponents<Transform>(-220, -250);
	logo->AddComponents<Render>();
	logo->SetParent(menuUI, false);

	auto pushStart = menuScene->CreateGameObject("startTextMenu");
	pushStart->AddComponents<Text>("PUSH ENTER BUTTON", mediumFont, SDL_Color{ 206, 206, 110, 255 }, true);
	pushStart->AddComponents<Transform>(0, 200);
	pushStart->AddComponents<Render>();
	pushStart->AddComponents<pacman::BlinkingText>();
	pushStart->SetParent(menuUI, false);

	//const std::vector<diji::Render*> textRenders = { 
	//	playerText->GetComponent<Render>(), 
	//	twoPlayerText->GetComponent<Render>(), 
	//	createLevel->GetComponent<Render>(), 
	//	options->GetComponent<Render>(), 
	//	highScoreDisplay->GetComponent<Render>(), 
	//	exitText->GetComponent<Render>(),
	//	//freeMode->GetComponent<Render>(),
	//	//ghostRules->GetComponent<Render>(),
	//};

	//menuUI->GetComponent<pacman::Menu>()->AddTextRender(textRenders);

	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::DPadDown, menuUI, pacman::MenuSwitch::MenuButtons::Down);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::DPadUp, menuUI, pacman::MenuSwitch::MenuButtons::Up);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::Start, menuUI, pacman::MenuSwitch::MenuButtons::Enter);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::A, menuUI, pacman::MenuSwitch::MenuButtons::Enter);

	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_DOWN, menuUI, pacman::MenuSwitch::MenuButtons::Down);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_S, menuUI, pacman::MenuSwitch::MenuButtons::Down);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_D, menuUI, pacman::MenuSwitch::MenuButtons::Down);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_UP, menuUI, pacman::MenuSwitch::MenuButtons::Up);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_W, menuUI, pacman::MenuSwitch::MenuButtons::Up);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_A, menuUI, pacman::MenuSwitch::MenuButtons::Up);
	input.BindCommand<pacman::MenuSwitch>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_RETURN, menuUI, pacman::MenuSwitch::MenuButtons::Enter);
}

void Loader::CommonGameAssets(Scene* &scene)
{
	Collision::GetInstance().Reset();

	auto background = scene->CreateGameObject("A_background");
	background->AddComponents<Texture>("BackgroundLevel.png");
	background->AddComponents<Transform>(0, 78);
	background->AddComponents<Render>(2);

	auto pelletCounter = scene->CreateGameObject("pelletCounter");
	pelletCounter->AddComponents<pacman::PelletObserver>();
	pelletCounter->AddComponents<pacman::PelletCounter>();

	auto player = scene->CreateGameObject("player");
	player->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player->GetComponent<Texture>()->SetRotation(true);
	player->AddComponents<Transform>(214, 439);
	player->AddComponents<Render>(2);
	player->AddComponents<pacman::HealthCounter>(4);
	player->AddComponents<pacman::ScoreCounter>(0);
	player->AddComponents<Collider>(15, 15);
	player->AddComponents<pacman::AI>();
	player->GetComponent<Render>()->DisableRender();
	player->GetComponent<Texture>()->PauseAnimation();
#pragma region LevelIntro
	const auto& font = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
	auto levelIntro = scene->CreateGameObject("levelIntro");
	levelIntro->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 250.f);
	levelIntro->AddComponents<pacman::LevelIntro>();

	auto PlayerText = scene->CreateGameObject("playerTextIntro");
	PlayerText->AddComponents<Transform>(0, 0);
	PlayerText->AddComponents<Text>("PLAYER ONE", font, SDL_Color{ 6, 232, 244, 255 }, 1);
	PlayerText->AddComponents<pacman::IntroTextObserver>(pacman::MessageTypesDerived::LEVEL_BEGIN);
	PlayerText->AddComponents<Render>();
	PlayerText->SetParent(levelIntro, false);

	auto readyText = scene->CreateGameObject("readyTextIntro");
	readyText->AddComponents<Transform>(0, 96);
	readyText->AddComponents<Text>("READY!", font, SDL_Color{ 255, 255, 0, 255 }, 1);
	readyText->AddComponents<pacman::IntroTextObserver>(pacman::MessageTypesDerived::LEVEL_START);
	readyText->AddComponents<Render>();
	readyText->SetParent(levelIntro, false);
#pragma endregion
#pragma region Ghosts
	auto GhostTimers = scene->CreateGameObject("ghostTimers");
	GhostTimers->AddComponents<pacman::GhostsTimers>();

	auto Blinky = scene->CreateGameObject("z_Blinky");
	Blinky->AddComponents<Texture>("RedGhost.png", 15, 15, 2);
	Blinky->AddComponents<Transform>(212, 247);
	Blinky->AddComponents<Render>(2);
	Blinky->AddComponents<Collider>(15, 15);
	Blinky->AddComponents<pacman::RedAI>(player, pelletCounter, GhostTimers);
	Blinky->AddComponents<pacman::GhostCollision>(player);
	Blinky->GetComponent<Render>()->DisableRender();

	auto Pinky = scene->CreateGameObject("z_Pinky");
	Pinky->AddComponents<Texture>("Pinky.png", 15, 15, 2);
	Pinky->AddComponents<Transform>(212, 300);
	Pinky->AddComponents<Render>(2);
	Pinky->AddComponents<Collider>(15, 15);
	Pinky->AddComponents<pacman::Pinky>(player, pelletCounter, GhostTimers);
	Pinky->AddComponents<pacman::GhostCollision>(player);
	Pinky->GetComponent<Render>()->DisableRender();

	auto Inky = scene->CreateGameObject("z_Inky");
	Inky->AddComponents<Texture>("Inky.png", 15, 15, 2);
	Inky->AddComponents<Transform>(180, 300);
	Inky->AddComponents<Render>(2);
	Inky->AddComponents<Collider>(15, 15);
	Inky->AddComponents<pacman::Inky>(player, pelletCounter, GhostTimers, Blinky);
	Inky->AddComponents<pacman::GhostCollision>(player);
	Inky->GetComponent<Render>()->DisableRender();

	auto Clyde = scene->CreateGameObject("z_Clyde");
	Clyde->AddComponents<Texture>("Clyde.png", 15, 15, 2);
	Clyde->AddComponents<Transform>(244, 300);
	Clyde->AddComponents<Render>(2);
	Clyde->AddComponents<Collider>(15, 15);
	Clyde->AddComponents<pacman::Clyde>(player, pelletCounter, GhostTimers);
	Clyde->AddComponents<pacman::GhostCollision>(player);
	Clyde->GetComponent<Render>()->DisableRender();

	const std::vector<GameObject*> ghosts = { Blinky, Pinky, Inky, Clyde };

	//Set the ghosts aware of each other
	const auto& BlinkyAI = Blinky->GetComponent<pacman::RedAI>();
	const auto& PinkyAI = Pinky->GetComponent<pacman::Pinky>();
	const auto& InkyAI = Inky->GetComponent<pacman::Inky>();
	const auto& ClydeAI = Clyde->GetComponent<pacman::Clyde>();
	const std::vector<pacman::GhostAI*> ghostAIs = { BlinkyAI, PinkyAI, InkyAI, ClydeAI };
	BlinkyAI->SetGhostsVector(ghostAIs);
	PinkyAI->SetGhostsVector(ghostAIs);
	InkyAI->SetGhostsVector(ghostAIs);
	ClydeAI->SetGhostsVector(ghostAIs);
#pragma endregion
#pragma region HUD
	const auto& textFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	auto HUD = scene->CreateGameObject("HUD");
	HUD->AddComponents<Transform>(10, 10);

	auto scoreCounter = scene->CreateGameObject("scoreCounterHUD");
	scoreCounter->AddComponents<Transform>(-5, 20);
	scoreCounter->AddComponents<pacman::ScoreObserver>("     00", textFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);

	auto playerText = scene->CreateGameObject("playerTextHUD");
	playerText->AddComponents<Transform>(65, 0);
	playerText->AddComponents<Text>("1UP", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	playerText->AddComponents<Render>();
	playerText->SetParent(HUD, false);

	auto highScoreText = scene->CreateGameObject("highScoreTextHUD");
	highScoreText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 10, 0.f);
	highScoreText->AddComponents<Text>("HIGH SCORE", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	highScoreText->AddComponents<Render>();
	highScoreText->SetParent(HUD, false);

	auto highScoreDisplay = scene->CreateGameObject("highScoreDisplayHUD");
	highScoreDisplay->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 75.f, 20.f);
	highScoreDisplay->AddComponents<pacman::HighScoreObserver>("  ", textFont, SDL_Color{ 255, 255, 255, 255 }, false);
	highScoreDisplay->AddComponents<Render>();
	highScoreDisplay->SetParent(HUD, false);

	auto livesCounter = scene->CreateGameObject("livesCounterHUD");
	livesCounter->AddComponents<Texture>("Lives.png", 80, 11, 1);
	livesCounter->GetComponent<Texture>()->PauseAnimation();
	livesCounter->AddComponents<Transform>(42, 581);
	livesCounter->AddComponents<pacman::PacmanHealthObserver>();
	livesCounter->AddComponents<Render>(2);

	auto levelCounterHUD = scene->CreateGameObject("levelCounterHUD");
	levelCounterHUD->AddComponents<Texture>("LevelDisplay.png", 16, 16, 1);
	levelCounterHUD->GetComponent<Texture>()->PauseAnimation();
	levelCounterHUD->GetComponent<Texture>()->SetStartingFrame(17);
	levelCounterHUD->AddComponents<Transform>(388, 573);
	levelCounterHUD->AddComponents<pacman::LevelObserver>();
	levelCounterHUD->AddComponents<pacman::LevelCounterRender>(2);
#pragma endregion

	const auto& pointFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 8);

	auto fruit = scene->CreateGameObject("w_fruit");
	fruit->AddComponents<Text>("100", pointFont, SDL_Color{ 255, 183, 255, 255 }, false);
	fruit->AddComponents<Texture>("LevelDisplay.png", 16, 16, 1);
	fruit->GetComponent<Texture>()->SetStartingFrame(17);
	fruit->GetComponent<Texture>()->PauseAnimation();
	fruit->AddComponents<Transform>(214, 343);
	fruit->AddComponents<pacman::FruitRender>(2);
	fruit->GetComponent<pacman::FruitRender>()->DisableRender();
	fruit->AddComponents<Collider>(16, 16);
	fruit->AddComponents<pacman::Fruit>(player, pelletCounter, levelCounterHUD);

#pragma region Commands
	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_W, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_A, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_S, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::KEYBOARD, KeyState::HELD, SDL_SCANCODE_D, player, Movement::Right);
	input.BindCommand<pacman::HitCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_C, player);

	input.BindCommand<pacman::SkipLevel>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_F1, pelletCounter);
	input.BindCommand<pacman::MuteCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_M, nullptr);
	input.BindCommand<pacman::MuteCommand>(PlayerIdx::PLAYER1, KeyState::PRESSED, Controller::Button::X, nullptr);

#pragma endregion
#pragma region Observers
	//todo: reset audio as well and add quick reset animation
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player->GetComponent<pacman::AI>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Blinky->GetComponent<pacman::RedAI>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Pinky->GetComponent<pacman::Pinky>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Inky->GetComponent<pacman::Inky>());
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), Clyde->GetComponent<pacman::Clyde>());
	//pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player->GetComponent<pacman::AI>());//one for level intro?
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), levelCounterHUD->GetComponent<pacman::LevelObserver>());

	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), PlayerText->GetComponent<pacman::IntroTextObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), readyText->GetComponent<pacman::IntroTextObserver>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), GhostTimers->GetComponent<pacman::GhostsTimers>());

	player->GetComponent<pacman::HealthCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::HEALTH_CHANGE), livesCounter->GetComponent<pacman::PacmanHealthObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), scoreCounter->GetComponent<pacman::ScoreObserver>());
	player->GetComponent<pacman::ScoreCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::SCORE_CHANGE), highScoreDisplay->GetComponent<pacman::HighScoreObserver>());
	fruit->GetComponent<pacman::Fruit>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::FRUIT_COLLISION), player->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), player->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), player->GetComponent<pacman::AI>());

	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Blinky->GetComponent<pacman::RedAI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Blinky->GetComponent<pacman::RedAI>());

	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Pinky->GetComponent<pacman::Pinky>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Pinky->GetComponent<pacman::Pinky>());

	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Inky->GetComponent<pacman::Inky>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Inky->GetComponent<pacman::Inky>());

	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player->GetComponent<pacman::AI>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), Clyde->GetComponent<pacman::Clyde>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), Clyde->GetComponent<pacman::Clyde>());
#pragma endregion
	const auto& smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);

	auto fpsCounter = scene->CreateGameObject("fpsCounter");
	fpsCounter->AddComponents<Text>("0 FPS", smallFont);
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->AddComponents<Transform>(static_cast<int>(loader::VIEWPORT.x - 75.f), 10);
	fpsCounter->AddComponents<Render>();
}

void Loader::PacmanLevel()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::LEVEL));

	CommonGameAssets(scene);
	
	//todo: ReadyText should spawn when player respawns
	auto& input = InputManager::GetInstance();
	const auto& player = scene->GetGameObject("player");

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player, Movement::Right);

	const std::vector<GameObject*> ghosts = { scene->GetGameObject("z_Blinky"), scene->GetGameObject("z_Pinky"), scene->GetGameObject("z_Inky"), scene->GetGameObject("z_Clyde") };
	pacman::PickUpLoader pickUpLoader{ scene->GetGameObject("player"), ghosts, scene->GetGameObject("pelletCounter"), scene };

}

void Loader::VersusLevel()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::VERSUS));

	CommonGameAssets(scene);

	scene->GetGameObject("playerTextHUD")->GetComponent<Text>()->SetText("2UP");
	
	const auto& player2 = scene->GetGameObject("z_Blinky");
	player2->GetComponent<pacman::RedAI>()->SetAsPlayer();

	const std::vector<GameObject*> ghosts = { scene->GetGameObject("z_Blinky"), scene->GetGameObject("z_Pinky"), scene->GetGameObject("z_Inky"), scene->GetGameObject("z_Clyde") };
	pacman::PickUpLoader pickUpLoader{ scene->GetGameObject("player"), ghosts, scene->GetGameObject("pelletCounter"), scene };

#pragma region Input
	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	input.BindCommand<pacman::GhostSwitchState>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);

	const auto& player = scene->GetGameObject("player");

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadUp, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadLeft, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadDown, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadRight, player, Movement::Right);
#pragma endregion
}

void Loader::CoopLevel()
{
	auto scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::COOP));

	CommonGameAssets(scene);

	scene->GetGameObject("playerTextHUD")->GetComponent<Text>()->SetText("2UP");

	auto player2 = scene->CreateGameObject("player2");
	player2->AddComponents<Texture>("pacmanSpriteSheet5.png", 15, 15, 4);
	player2->GetComponent<Texture>()->SetRotation(true);
	player2->AddComponents<Transform>(214, 439);
	player2->GetComponent<Transform>()->SetMovement(diji::Movement::Left);
	player2->AddComponents<Render>(2);
	player2->AddComponents<pacman::HealthCounter>(4);
	player2->AddComponents<pacman::ScoreCounter>(0);
	player2->GetComponent<pacman::ScoreCounter>()->SetAsPlayer2();
	player2->AddComponents<Collider>(15, 15);
	player2->AddComponents<pacman::AI>();
	player2->GetComponent<pacman::AI>()->SetInitialMovementLeft();
	player2->GetComponent<Render>()->DisableRender();
	player2->GetComponent<Texture>()->PauseAnimation();
	player2->GetComponent<Texture>()->SetRotationAngle(180.f);
	

	const auto& Blinky = scene->GetGameObject("z_Blinky");
	const auto& Pinky = scene->GetGameObject("z_Pinky");
	const auto& Inky = scene->GetGameObject("z_Inky");
	const auto& Clyde = scene->GetGameObject("z_Clyde");

	Blinky->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());
	Inky->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddPLayer2Collider(player2->GetComponent<Collider>());

	const std::vector<GameObject*> ghosts = { Blinky, Pinky, Inky, Clyde };
	pacman::PickUpLoader pickUpLoader{ scene->GetGameObject("player"), ghosts, scene->GetGameObject("pelletCounter"), scene, player2 };

	const auto& pelletCounter = scene->GetGameObject("pelletCounter");
	pelletCounter->GetComponent<pacman::PelletCounter>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_END), player2->GetComponent<pacman::AI>());

	const auto& fruit = scene->GetGameObject("w_fruit");
	fruit->GetComponent<pacman::Fruit>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::FRUIT_COLLISION), player2->GetComponent<pacman::AI>());
	fruit->GetComponent<pacman::Fruit>()->AddPlayer2Collider(player2->GetComponent<Collider>());

	const auto& levelIntro = scene->GetGameObject("levelIntro");
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_BEGIN), player2->GetComponent<pacman::AI>());
	levelIntro->GetComponent<pacman::LevelIntro>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::LEVEL_START), player2->GetComponent<pacman::AI>());

	Blinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());
	Pinky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());
	Inky->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());
	Clyde->GetComponent<pacman::GhostCollision>()->AddObserver(static_cast<MessageTypes>(pacman::MessageTypesDerived::ENEMY_COLLISION), player2->GetComponent<pacman::AI>());

#pragma region Input
	auto& input = InputManager::GetInstance();

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadUp, player2, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadLeft, player2, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadDown, player2, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER1, KeyState::HELD, Controller::Button::DPadRight, player2, Movement::Right);

	const auto& player = scene->GetGameObject("player");

	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadUp, player, Movement::Up);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadLeft, player, Movement::Left);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadDown, player, Movement::Down);
	input.BindCommand<pacman::Move>(PlayerIdx::PLAYER2, KeyState::HELD, Controller::Button::DPadRight, player, Movement::Right);
#pragma endregion
}

void Loader::HighScoreMenu(int score)
{
	InputManager::GetInstance().ResetCommands();
	const auto& scene = SceneManager::GetInstance().CreateScene(static_cast<int>(pacman::GameState::GAMEOVER));
	const auto& mediumFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 18);

	auto topTenText = scene->CreateGameObject("topTenText");
	topTenText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, 80.f);
	topTenText->AddComponents<Text>("The 10 BEST PLAYERS!", mediumFont, SDL_Color{ 255, 255, 255, 255 }, true);
	topTenText->AddComponents<Render>();

	auto enterYourName = scene->CreateGameObject("enterYourNameText");
	enterYourName->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, loader::VIEWPORT.y - 140.f);
	enterYourName->AddComponents<Text>("ENTER YOUR NAME:", mediumFont, SDL_Color{ 255, 255, 255, 255 }, true);
	enterYourName->AddComponents<Render>();
	enterYourName->AddComponents<pacman::BlinkingText>();

	auto scoresTexts = scene->CreateGameObject("scoresTexts");
	scoresTexts->AddComponents<Transform>(loader::VIEWPORT.x * 0.24f , 120.f);
	scoresTexts->AddComponents<pacman::HighScoreRender>();

	auto enterName = scene->CreateGameObject("name");
	enterName->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f, loader::VIEWPORT.y - 100.f);
	enterName->AddComponents<Text>("AAA", mediumFont, SDL_Color{ 255, 255, 255, 255 }, true);
	enterName->AddComponents<pacman::EnterName>(score);
	enterName->AddComponents<pacman::CustomTextRender>();

	const auto& textFont = ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);

	auto HUD = scene->CreateGameObject("HUD");
	HUD->AddComponents<Transform>(10, 10);

	auto scoreCounter = scene->CreateGameObject("scoreCounterHUD");
	scoreCounter->AddComponents<Transform>(-5, 20);
	const int numDigits = score > 0 ? static_cast<int>(std::log10(score)) + 1 : 1;
	const int numSpaces = std::max(0, 7 - numDigits);
	const std::string formattedScore = std::format("{:>{}}", score, numSpaces + numDigits);
	scoreCounter->AddComponents<pacman::ScoreObserver>(formattedScore, textFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD, false);

	auto highScoreText = scene->CreateGameObject("highScoreTextHUD");
	highScoreText->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 10, 0.f);
	highScoreText->AddComponents<Text>("HIGH SCORE", textFont, SDL_Color{ 255, 255, 255, 255 }, true);
	highScoreText->AddComponents<Render>();
	highScoreText->SetParent(HUD, false);

	auto highScoreDisplay = scene->CreateGameObject("highScoreDisplayHUD");
	highScoreDisplay->AddComponents<Transform>(loader::VIEWPORT.x * 0.5f - 75.f, 20.f);
	highScoreDisplay->AddComponents<pacman::HighScoreObserver>("  ", textFont, SDL_Color{ 255, 255, 255, 255 }, false);
	highScoreDisplay->GetComponent<pacman::HighScoreObserver>()->DisplayPossibleScore(score);
	highScoreDisplay->AddComponents<Render>();
	highScoreDisplay->SetParent(HUD, false);

	//input
	auto& input = InputManager::GetInstance();
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_W, enterName, Movement::Up);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_A, enterName, Movement::Left);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_S, enterName, Movement::Down);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_D, enterName, Movement::Right);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_UP, enterName, Movement::Up);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_LEFT, enterName, Movement::Left);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_DOWN, enterName, Movement::Down);
	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_RIGHT, enterName, Movement::Right);

	input.BindCommand<pacman::NameChangeCommand>(PlayerIdx::KEYBOARD, KeyState::RELEASED, SDL_SCANCODE_RETURN, enterName, Movement::Idle);
}

void Loader::Load()
{
#ifndef NDEBUG
	ServiceLocator::RegisterSoundSystem(std::make_unique<LoggingSoundSystem>(std::make_unique<SDLISoundSystem>()));
#else
	ServiceLocator::RegisterSoundSystem(std::make_unique<SDLISoundSystem>());
#endif
	pacman::ScoreBoard::GetInstance().Init();

	PacmanIntro();

	Collision::GetInstance().ParseLevelSVG("BackgroundLevelBlack.svg", 78);
	Collision::GetInstance().ParseIntersectionsSVG("Intersections.svg", 78);

	SceneManager::GetInstance().SetActiveScene(static_cast<int>(pacman::GameState::INTRO));
}
