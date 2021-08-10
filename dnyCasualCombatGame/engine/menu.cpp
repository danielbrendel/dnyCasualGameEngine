#include "menu.h"
#include "game.h"

/*
	Casual Combat Game (dnyCasualCombatGame) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

namespace Menu {
	void MainMenu_onOpenNews(void)
	{
		std::cout << "MainMenu_onOpenNews" << std::endl;
	}

	void MainMenu_onOpenPackages(void)
	{
		std::cout << "MainMenu_onOpenPackages" << std::endl;
	}

	void MainMenu_onOpenSettings(void)
	{
		std::cout << "MainMenu_onOpenSettings" << std::endl;

	}

	void MainMenu_onQuitGame(void)
	{
		Game::pGame->Release();
	}
}