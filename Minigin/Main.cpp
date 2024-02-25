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
	auto& scene = diji::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_shared<diji::GameObject>();
	background->AddComponents<diji::Texture>("background.tga");
	scene.Add(background);

	auto logo = std::make_shared<diji::GameObject>();
	logo->AddComponents<diji::Texture>("logo.tga");
	logo->GetComponent<diji::Texture>()->SetPosition(300, 140);
	scene.Add(logo);

	auto font = diji::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
	auto title = std::make_shared<diji::GameObject>();
	title->AddComponents<diji::Texture>("Programming 4 Assignment", font);
	title->GetComponent<diji::Texture>()->SetPosition(220, 20);
	scene.Add(title);

	//need to rework the classes, its members names and constructor/destructor placement
	// need to add everything in namespaces
	// add a RenderComponent and TransformComponent maybe separate the Texture and Text components
	auto fpsCounter = std::make_shared<diji::GameObject>();
	fpsCounter->AddComponents<diji::FPSCounter>();

	std::string fpsString{};
	if (fpsCounter->HasComponent<diji::FPSCounter>())
		fpsString = std::to_string(fpsCounter->GetComponent<diji::FPSCounter>()->GetFPS()) + " FPS";
	
	fpsCounter->AddComponents<diji::Texture>(fpsString, font);
	fpsCounter->GetComponent<diji::Texture>()->SetPosition(0, 10);
	scene.Add(fpsCounter);
}

int main(int, char*[]) {
	diji::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}