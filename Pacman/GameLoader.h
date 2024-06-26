#pragma once

namespace diji
{
	class Scene;
}

namespace Loader 
{
	void PacmanMenu();
	void CommonGameAssets(diji::Scene* &scene);
	void PacmanLevel();
	void VersusLevel();
	void CoopLevel();
	void HighScoreMenu(int score = 0);
	void Load();

}

