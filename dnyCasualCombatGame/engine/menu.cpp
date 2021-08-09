#include "menu.h"
#include "game.h"

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