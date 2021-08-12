#pragma once

/*
	Casual Game Engine (dnyCasualGameEngine) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "shared.h"
#include "vars.h"

/* Menu component */
namespace Menu {
	void MainMenu_OnResumeGame(void);
	void MainMenu_OnStopGame(void);
	void MainMenu_OnOpenNews(void);
	void MainMenu_OnOpenPackages(void);
	void MainMenu_OnOpenSettings(void);
	void MainMenu_OnQuitGame(void);

	class IMenu {
	protected:
		bool m_bActive;
		int m_iMouseX;
		int m_iMouseY;
	public:
		IMenu() : m_bActive(false) {}

		virtual bool Initialize(int w, int h, bool* pGameStarted) = 0;
		virtual void Draw(void) = 0;
		virtual void Release(void) = 0;

		//Called for mouse events
		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			if (!iMouseKey) {
				this->m_iMouseX = x;
				this->m_iMouseY = y;
			}
		}

		virtual void SetActiveStatus(bool status)
		{
			this->m_bActive = status;
		}

		virtual bool IsActive(void)
		{
			return this->m_bActive;
		}
	};

	#define MAINMENU_FONTSIZE_W 15
	#define MAINMENU_FONTSIZE_H 20
	class CMainMenu : public IMenu {
	private:
		typedef void (*TOnClick)(void);

		struct menuentry_s {
			int x, y;
			std::wstring wszText;
			TOnClick onClick;
			bool bHover;
			bool bShow;
		};

		std::vector<menuentry_s> m_vEntries;
		DxRenderer::d3dfont_s* m_pFont;
		bool* m_pGameStarted;
	public:
		CMainMenu() {}
		~CMainMenu() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize main menu
			
			const int iStartPos = 50;

			menuentry_s sResumeGame;
			sResumeGame.wszText = L"Resume game";
			sResumeGame.onClick = &MainMenu_OnResumeGame;
			sResumeGame.x = 10;
			sResumeGame.y = h - iStartPos - 50 * 6;
			sResumeGame.bShow = false;

			menuentry_s sStopGame;
			sStopGame.wszText = L"Stop game";
			sStopGame.onClick = &MainMenu_OnStopGame;
			sStopGame.x = 10;
			sStopGame.y = h - iStartPos - 50 * 5;
			sStopGame.bShow = false;

			menuentry_s sNews;
			sNews.wszText = L"News";
			sNews.onClick = &MainMenu_OnOpenNews;
			sNews.x = 10;
			sNews.y = h - iStartPos - 50 * 3;
			sNews.bShow = true;

			menuentry_s sPackages;
			sPackages.wszText = L"Packages";
			sPackages.onClick = &MainMenu_OnOpenPackages;
			sPackages.x = 10;
			sPackages.y = h - iStartPos - 50 * 2;
			sPackages.bShow = true;

			menuentry_s sSettings;
			sSettings.wszText = L"Settings";
			sSettings.onClick = &MainMenu_OnOpenSettings;
			sSettings.x = 10;
			sSettings.y = h - iStartPos - 50;
			sSettings.bShow = true;

			menuentry_s sQuit;
			sQuit.wszText = L"Quit";
			sQuit.onClick = &MainMenu_OnQuitGame;
			sQuit.x = 10;
			sQuit.y = h - iStartPos;
			sQuit.bShow = true;

			this->m_vEntries.push_back(sResumeGame);
			this->m_vEntries.push_back(sStopGame);
			this->m_vEntries.push_back(sNews);
			this->m_vEntries.push_back(sPackages);
			this->m_vEntries.push_back(sSettings);
			this->m_vEntries.push_back(sQuit);

			this->m_pFont = pRenderer->LoadFont(L"Verdana", MAINMENU_FONTSIZE_W, MAINMENU_FONTSIZE_H);
			if (!this->m_pFont) {
				return false;
			}

			this->m_pGameStarted = pGameStarted;

			return true;
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			IMenu::OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

			for (size_t i = 0; i < this->m_vEntries.size(); i++) {
				if (this->m_vEntries[i].bShow) {
					if ((this->m_iMouseX >= this->m_vEntries[i].x) && (this->m_iMouseX <= this->m_vEntries[i].x + MAINMENU_FONTSIZE_W * this->m_vEntries[i].wszText.length()) && (this->m_iMouseY >= this->m_vEntries[i].y) && (this->m_iMouseY <= this->m_vEntries[i].y + MAINMENU_FONTSIZE_H)) {
						this->m_vEntries[i].bHover = true;

						if ((iMouseKey == 1) && (!bDown)) {
							this->m_vEntries[i].onClick();
						}
					} else {
						this->m_vEntries[i].bHover = false;
					}
				}
			}
		}

		virtual void Draw(void)
		{
			//Draw menu

			for (size_t i = 0; i < this->m_vEntries.size(); i++) {
				if (this->m_vEntries[i].bShow) {
					if (this->m_vEntries[i].bHover) {
						pRenderer->DrawString(this->m_pFont, this->m_vEntries[i].wszText, this->m_vEntries[i].x, this->m_vEntries[i].y, 230, 230, 230, 150);
					}
					else {
						pRenderer->DrawString(this->m_pFont, this->m_vEntries[i].wszText, this->m_vEntries[i].x, this->m_vEntries[i].y, 200, 200, 200, 150);
					}
				}
			}
		}

		virtual void Release(void)
		{
			//Release component
		}

		virtual void OnToggleGameActiveMenuItems(void)
		{
			//Toggle game-active menu specific items

			this->m_vEntries[0].bShow = this->m_vEntries[1].bShow = *this->m_pGameStarted;
		}
	};

	class CMenu {
	private:
		CMainMenu oMainMenu;
		bool m_bOpen;
		DxRenderer::HD3DSPRITE m_hCursor;
		int m_iMouseX;
		int m_iMouseY;
	public:
		CMenu() : m_bOpen(false) {}
		~CMenu() {}

		bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize menu

			this->m_hCursor = pRenderer->LoadSprite(wszBasePath + L"media\\menucursor.png", 1, 16, 16, 1, false);
			if (this->m_hCursor == GFX_INVALID_SPRITE_ID) {
				return false;
			}

			if (!oMainMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			oMainMenu.SetActiveStatus(true);

			return true;
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!this->m_bOpen)
				return;

			if (!iMouseKey) {
				this->m_iMouseX = x;
				this->m_iMouseY = y;
			}

			oMainMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		void OnMouseWheel(short wDistance, bool bForward)
		{
			//Handle mouse wheel event
		}

		void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handke key events
		}

		void Draw(void)
		{
			//Draw menu

			if (!this->m_bOpen)
				return;
			
			pRenderer->DrawSprite(this->m_hCursor, this->m_iMouseX, this->m_iMouseY, 0, 0.0f);

			if (oMainMenu.IsActive()) {
				oMainMenu.Draw();
			}
		}

		void Release(void)
		{
			//Release menu

			oMainMenu.Release();
		}

		void SetOpenStatus(bool status)
		{
			this->m_bOpen = status;

			if (status) {
				oMainMenu.OnToggleGameActiveMenuItems();
			}
		}

		void OnStopGame(void)
		{
			oMainMenu.OnToggleGameActiveMenuItems();
		}

		bool IsOpen(void)
		{
			return this->m_bOpen;
		}
	};
}