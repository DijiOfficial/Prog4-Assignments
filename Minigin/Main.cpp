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
#include "TextObject.h"
#include "Scene.h"


#include "GameObject.h"
#include "FPSCounter.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	go->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	go->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	scene.Add(to);

	//need to add texture as component
	//need to rework the classes, its members names and constructor/destructor placement
	//remove all temporary references to textures i just added to make it work
	auto fpsCounter = std::make_shared<GameObject>();
	fpsCounter->AddComponents<FPSCounter>();
	fpsCounter->SetNoTexture();
	scene.Add(fpsCounter);

	std::string fpsString{};
	if (fpsCounter->HasComponent<FPSCounter>())
		fpsString = std::to_string(fpsCounter->GetComponent<FPSCounter>()->GetFPS());
	else
		fpsString = "no fps counter";

	auto fps = std::make_shared<TextObject>(fpsString, font);
	fps->SetPosition(0, 20);
	fps->SetNeedsFrameUpdate();
	scene.Add(fps);
}

int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}