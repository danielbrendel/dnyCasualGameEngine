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
	void CPlayMenu::OnButtonClick(class CButton* pButton)
	{
		if (pButton == &this->m_oPlay) {
			Game::pGame->InitStartGame(L"game");
		} else if (pButton == &this->m_oLoad) {
			if (this->m_oSaveGames.GetSelectedItem() != std::string::npos) {
				Game::pGame->LoadSavedGameState(this->m_oSaveGames.GetItem(this->m_oSaveGames.GetSelectedItem()));
			} else {
				Game::pGame->AddHudInfoMessage(L"Please select a saved game state", Entity::HudMessageColor::HM_RED, 5000);
			}
		}
	}

	void CPlayMenu::OnListBoxSelectEvent(class CListBox* pListBox, size_t uiItem)
	{
	}

	void CPackageMenu::OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		this->m_oButton.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		this->m_oImageListView.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		this->m_oBrowse.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
	}

	void CPackageMenu::OnButtonClick(class CButton* pButton)
	{
		if (pButton == &this->m_oButton) {
			if ((this->m_uiSelectedPackage >= 0) && (this->m_uiSelectedPackage < this->m_vPackages.size())) {
				Game::pGame->InitStartGame(this->m_vPackages[this->m_uiSelectedPackage].wszIdent, this->m_vPackages[this->m_uiSelectedPackage].wszPath);
				this->m_uiSelectedPackage = std::string::npos;
			}
		} else if (pButton == &this->m_oBrowse) {
			SteamFriends()->ActivateGameOverlayToWebPage(std::string("https://steamcommunity.com/app/" + std::to_string(pAppSteamID->iValue - 1) + "/workshop/").c_str());
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

	void CSettingsKeys::OnButtonClick(class CButton* pButton)
	{
		if (pButton == &this->m_btnSave) {

		} else if (pButton == &this->m_btnRestoreDefaults) {

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

	void MainMenu_OnOpenPlay(class CMenu* pMenu)
	{
		pMenu->OpenPlayMenu();
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

	void CIntermissionMenu::OnButtonClick(class CButton* pButton)
	{
		Game::pGame->GetCursor()->SetActiveStatus(false);
		
		if (this->m_bGameFinished) {
			pConsole->AddLine(L"Game has finished!");
			Game::pGame->StopGame();
		} else {
			Game::pGame->LoadMap(Game::pGame->GetGoalEntity()->GetGoal() + L".cfg");
		}
	}

	void CGameOverMenu::OnButtonClick(class CButton* pButton)
	{
		Game::pGame->GetCursor()->SetActiveStatus(false);

		if (pButton == &this->m_oBtnRestart) {
			Game::pGame->InitRestartGame();
		} else if (pButton == &this->m_oBtnLoadLastSavedGame) {
			Game::pGame->QuickLoad();
		} else if (pButton == &this->m_oBtnReturnToMainMenu) {
			Game::pGame->StopGame();
		}
	}
}