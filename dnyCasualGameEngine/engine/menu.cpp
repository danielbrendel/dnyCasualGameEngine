#include "menu.h"
#include "game.h"

/*
	Casual Game Engine (dnyCasualGameEngine) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

namespace Menu {
	void MainMenu_OnResumeGame(void)
	{
		Game::pGame->ResumeGame();
	}

	void MainMenu_OnStopGame(void)
	{
		Game::pGame->StopGame();
	}

	void MainMenu_OnOpenNews(void)
	{
		pConsole->AddLine(L"MainMenu_onOpenNews");
	}

	void MainMenu_OnOpenPackages(void)
	{
		pConsole->AddLine(L"MainMenu_onOpenPackages");
	}

	void MainMenu_OnOpenSettings(void)
	{
		pConsole->AddLine(L"MainMenu_onOpenSettings");
	}

	void MainMenu_OnQuitGame(void)
	{
		Game::pGame->Release();
	}
}