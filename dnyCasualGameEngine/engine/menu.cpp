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
	void CPackageMenu::OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		if (!iMouseKey) {
			this->m_iMouseX = x;
			this->m_iMouseY = y;
		}
		else {
			if ((iMouseKey == 1) && (this->m_bPlayButtonHover)) {
				Game::pGame->StartGame(this->m_vPackages[this->m_uiSelectedPackage].wszIdent);
			}
		}
	}

	void MainMenu_OnResumeGame(class CMenu* pMenu)
	{
		Game::pGame->ResumeGame();
	}

	void MainMenu_OnStopGame(class CMenu* pMenu)
	{
		Game::pGame->StopGame();
	}

	void MainMenu_OnOpenNews(class CMenu* pMenu)
	{
		pConsole->AddLine(L"MainMenu_onOpenNews");
	}

	void MainMenu_OnOpenPackages(class CMenu* pMenu)
	{
		pMenu->OpenPackageMenu();
	}

	void MainMenu_OnOpenSettings(class CMenu* pMenu)
	{
		pConsole->AddLine(L"MainMenu_onOpenSettings");
	}

	void MainMenu_OnQuitGame(class CMenu* pMenu)
	{
		Game::pGame->Release();
	}
}