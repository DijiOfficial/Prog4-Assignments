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

#include "GameObject.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_shared<GameObject>();
	background->AddComponents<Texture>("background.tga");
	scene.Add(background);

	auto logo = std::make_shared<GameObject>();
	logo->AddComponents<Texture>("logo.tga");
	logo->GetComponent<Texture>()->SetPosition(300, 140);
	scene.Add(logo);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto title = std::make_shared<GameObject>();
	title->AddComponents<Texture>("Programming 4 Assignment", font);
	title->GetComponent<Texture>()->SetPosition(220, 20);
	scene.Add(title);

	//need to rework the classes, its members names and constructor/destructor placement
	// need to add everything in namespaces
	// add a RenderComponent and TransformComponent maybe separate the Texture and Text components
	auto fpsCounter = std::make_shared<GameObject>();
	fpsCounter->AddComponents<FPSCounter>();

	std::string fpsString{};
	if (fpsCounter->HasComponent<FPSCounter>())
		fpsString = std::to_string(fpsCounter->GetComponent<FPSCounter>()->GetFPS()) + " FPS";
	
	fpsCounter->AddComponents<Texture>(fpsString, font);
	fpsCounter->GetComponent<Texture>()->SetPosition(0, 10);
	scene.Add(fpsCounter);
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}