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
	void MainMenu_onOpenNews(void)
	{
		pConsole->AddLine(L"MainMenu_onOpenNews");
	}

	void MainMenu_onOpenPackages(void)
	{
		pConsole->AddLine(L"MainMenu_onOpenPackages");
	}

	void MainMenu_onOpenSettings(void)
	{
		pConsole->AddLine(L"MainMenu_onOpenSettings");
	}

	void MainMenu_onQuitGame(void)
	{
		Game::pGame->Release();
	}
}