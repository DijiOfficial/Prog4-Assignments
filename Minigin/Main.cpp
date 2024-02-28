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
#include "GameObject.h"

using namespace diji;
void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	//Bakcground
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
	fpsCounter->AddComponents<FPSCounter>();

	std::string fpsString{};
	if (fpsCounter->HasComponent<FPSCounter>())
		fpsString = std::to_string(fpsCounter->GetComponent<FPSCounter>()->GetFPS()) + " FPS";
	
	fpsCounter->AddComponents<Text>(fpsString, font);
	fpsCounter->AddComponents<Transform>(0, 10);
	fpsCounter->AddComponents<Render>();
	scene.Add(fpsCounter);
	
	//Pac
	auto pacman = std::make_shared<GameObject>();
	pacman->AddComponents<Texture>("Pacs.png", 14, 14, 0);
	pacman->AddComponents<Transform>(150, 150);
	pacman->AddComponents<Rotation>(-300.f, 35.f);
	pacman->AddComponents<Render>();
	scene.Add(pacman);

	auto redGhost = std::make_shared<GameObject>();
	redGhost->AddComponents<Texture>("Pacs.png", 14, 14, 1);
	redGhost->AddComponents<Transform>();
	redGhost->AddComponents<Rotation>(400.f, 35.f);
	redGhost->AddComponents<Render>();
	redGhost->SetParent(pacman.get(), true);
	scene.Add(redGhost);

	auto blueGhost = std::make_shared<GameObject>();
	blueGhost->AddComponents<Texture>("Pacs.png", 14, 14, 2);
	blueGhost->AddComponents<Transform>(20,20);
	blueGhost->AddComponents<Rotation>(-300.f, 35.f);
	blueGhost->AddComponents<Render>();
	blueGhost->SetParent(redGhost.get(), true);
	scene.Add(blueGhost);

	auto ghost = std::make_shared<GameObject>();
	ghost->AddComponents<Texture>("Pacs.png", 14, 14, 3);
	ghost->AddComponents<Transform>();
	ghost->AddComponents<Rotation>(400.f, -35.f);
	ghost->AddComponents<Render>();
	ghost->SetParent(pacman.get(), true);
	scene.Add(ghost);
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}