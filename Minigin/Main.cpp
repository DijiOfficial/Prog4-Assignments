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

	////Bakcground
	//auto background = std::make_shared<GameObject>();
	//background->AddComponents<Texture>("background.tga");
	//background->AddComponents<Render>();
	//scene.Add(background);

	////Logo
	//auto logo = std::make_shared<GameObject>();
	//logo->AddComponents<Texture>("logo.tga");
	//logo->AddComponents<Transform>(300, 140);
	//logo->AddComponents<Render>();
	//scene.Add(logo);

	////Title
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	//auto title = std::make_shared<GameObject>();
	//title->AddComponents<Text>("Programming 4 Assignment", font);
	//title->AddComponents<Transform>(220, 20);
	//title->AddComponents<Render>();
	//scene.Add(title);

	////FPS Counter
	//auto fpsCounter = std::make_shared<GameObject>();
	//fpsCounter->AddComponents<Text>("0 FPS", font);
	//fpsCounter->AddComponents<FPSCounter>();
	//fpsCounter->AddComponents<Transform>(0, 10);
	//fpsCounter->AddComponents<Render>();
	//scene.Add(fpsCounter);	

	//TrashTheCache
	auto trashTheCache = std::make_shared<GameObject>();
	trashTheCache->AddComponents<TrashTheCache>(1);
	scene.Add(trashTheCache);

	auto trashTheCache2 = std::make_shared<GameObject>();
	trashTheCache2->AddComponents<TrashTheCache>(2);
	scene.Add(trashTheCache2);
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}