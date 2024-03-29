#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Components.h"
//#include "GameObject.h"
#include "InputManager.h"


using namespace diji;
void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	auto& subject = ISubject::GetInstance();

#pragma region Base
	//Background
	auto background = std::make_shared<GameObject>();
	background->AddComponents<Texture>("background.tga");
	background->AddComponents<Render>();
	scene.Add(background);

	//Logo
	auto logo = std::make_shared<GameObject>();
	logo->AddComponents<Texture>("logo.tga");
	logo->AddComponents<Transform>(300, 140);
	logo->AddComponents<Render>();
	scene.Add(logo);

	//Title
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto title = std::make_shared<GameObject>();
	title->AddComponents<Text>("Programming 4 Assignment", font);
	title->AddComponents<Transform>(220, 20);
	title->AddComponents<Render>();
	scene.Add(title);

	//FPS Counter
	auto fpsCounter = std::make_shared<GameObject>();
	fpsCounter->AddComponents<Text>("0 FPS", font);
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->AddComponents<Transform>(0, 10);
	fpsCounter->AddComponents<Render>();
	scene.Add(fpsCounter);	
#pragma endregion

#pragma region Players
	//Player1
	auto player = std::make_shared<GameObject>();
	player->AddComponents<Texture>("pacmanSpriteSheet.png", 13, 13, 4);
	player->AddComponents<Transform>(330, 250);
	player->AddComponents<Render>();
	player->AddComponents<HealthCounter>(3);
	player->AddComponents<ScoreCounter>(0);

	auto& input = InputManager::GetInstance();
	input.BindKeyboard(player.get());
	scene.Add(player);

	//Player2
	auto player2 = std::make_shared<GameObject>();
	player2->AddComponents<Texture>("redGhost.png", 13, 13);
	player2->AddComponents<Transform>(300, 250);
	player2->AddComponents<Render>();
	player2->AddComponents<HealthCounter>(3);
	player2->AddComponents<ScoreCounter>(0);


	input.BindController(player2.get(), 0);
	scene.Add(player2);
#pragma endregion

#pragma region HUD
	auto smallFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 14);
	auto HUD = std::make_shared<GameObject>();
	HUD->AddComponents<Transform>(0, 70);

	auto controllerInfo = std::make_shared<GameObject>();
	controllerInfo->AddComponents<Transform>(0, 0);
	controllerInfo->AddComponents<Text>("Use the D-Pad to move Pacman, X to inflict damage, A and B to pick up pellets", smallFont);
	controllerInfo->AddComponents<Render>();
	controllerInfo->SetParent(HUD.get(), false);

	auto controlsInfo = std::make_shared<GameObject>();
	controlsInfo->AddComponents<Transform>(0, 20);
	controlsInfo->AddComponents<Text>("Use the WASD to move MS Pacman, C to inflict damage, Z and X to pick up pellets", smallFont);
	controlsInfo->AddComponents<Render>();
	controlsInfo->SetParent(HUD.get(), false);

	auto livesCounter = std::make_shared<GameObject>();
	livesCounter->AddComponents<Transform>(0, 60);
	livesCounter->AddComponents<Text>("# lives: 3", smallFont);
	livesCounter->AddComponents<Render>();
	livesCounter->SetParent(HUD.get(), false);

	std::unique_ptr<IObserver> healthObserver = std::make_unique<HealthObserver>(livesCounter.get(), player.get());
	subject.AddObserver(MessageTypes::ENTITY_COLLISION, std::move(healthObserver));

	auto scoreCounter = std::make_shared<GameObject>();
	scoreCounter->AddComponents<Transform>(0, 80);
	scoreCounter->AddComponents<Text>("Score: 0", smallFont);
	scoreCounter->AddComponents<Render>();
	scoreCounter->SetParent(HUD.get(), false);

	std::unique_ptr<IObserver> scoreObserver = std::make_unique<ScoreObserver>(scoreCounter.get(), player.get());
	subject.AddObserver(MessageTypes::SCORE_UPDATE, std::move(scoreObserver));

	auto livesCounter2 = std::make_shared<GameObject>();
	livesCounter2->AddComponents<Transform>(0, 100);
	livesCounter2->AddComponents<Text>("# lives: 3", smallFont);
	livesCounter2->AddComponents<Render>();
	livesCounter2->SetParent(HUD.get(), false);

	std::unique_ptr<IObserver> healthObserver2 = std::make_unique<HealthObserver>(livesCounter2.get(), player2.get());
	subject.AddObserver(MessageTypes::ENTITY_COLLISION, std::move(healthObserver2));

	auto scoreCounter2 = std::make_shared<GameObject>();
	scoreCounter2->AddComponents<Transform>(0, 120);
	scoreCounter2->AddComponents<Text>("Score: 0", smallFont);
	scoreCounter2->AddComponents<Render>();
	scoreCounter2->SetParent(HUD.get(), false);

	std::unique_ptr<IObserver> scoreObserver2 = std::make_unique<ScoreObserver>(scoreCounter2.get(), player2.get());
	subject.AddObserver(MessageTypes::SCORE_UPDATE, std::move(scoreObserver2));

	scene.Add(HUD);
	scene.Add(livesCounter);
	scene.Add(scoreCounter);
	scene.Add(livesCounter2);
	scene.Add(scoreCounter2);
	scene.Add(controllerInfo);
	scene.Add(controlsInfo);
#pragma endregion


}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}