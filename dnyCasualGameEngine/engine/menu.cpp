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
		this->m_oButton.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		this->m_oImageListView.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
	}

	void CPackageMenu::OnButtonClick(class CButton* pButton)
	{
		if (pButton == &this->m_oButton) {
			Game::pGame->StartGame(this->m_vPackages[this->m_uiSelectedPackage].wszIdent);
		}
	}

	void CPackageMenu::OnImageSelected(CImageListView* pImageListView, size_t uiItemId)
	{
		const CImageListView::viewitem_s& viewItem = pImageListView->GetItem(uiItemId);

		for (size_t i = 0; i < this->m_vPackages.size(); i++) {
			if (this->m_vPackages[i].wszIdent == viewItem.wszIdent) {
				this->m_uiSelectedPackage = i;
				break;
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
		pConsole->AddLine(L"MainMenu_OnOpenNews");
	}

	void MainMenu_OnOpenPackages(class CMenu* pMenu)
	{
		pMenu->OpenPackageMenu();
	}

	void MainMenu_OnOpenSettings(class CMenu* pMenu)
	{
		pMenu->OpenSettingsMenu();
	}

	void MainMenu_OnQuitGame(class CMenu* pMenu)
	{
		Game::pGame->Release();
	}
}