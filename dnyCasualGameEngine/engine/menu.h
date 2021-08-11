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
	void MainMenu_onOpenNews(void);
	void MainMenu_onOpenPackages(void);
	void MainMenu_onOpenSettings(void);
	void MainMenu_onQuitGame(void);

	class IMenu {
	protected:
		bool m_bActive;
		int m_iMouseX;
		int m_iMouseY;
	public:
		IMenu() : m_bActive(false) {}

		virtual bool Initialize(int w, int h) = 0;
		virtual void Draw(void) = 0;
		virtual void Release(void) = 0;

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
		};

		std::vector<menuentry_s> m_vEntries;
		DxRenderer::d3dfont_s* m_pFont;
	public:
		CMainMenu() {}
		~CMainMenu() {}

		virtual bool Initialize(int w, int h)
		{
			//Initialize main menu
			
			const int iStartPos = 50;

			menuentry_s sNews;
			sNews.wszText = L"News";
			sNews.onClick = &MainMenu_onOpenNews;
			sNews.x = 10;
			sNews.y = h - iStartPos - 50 * 3;

			menuentry_s sPackages;
			sPackages.wszText = L"Packages";
			sPackages.onClick = &MainMenu_onOpenPackages;
			sPackages.x = 10;
			sPackages.y = h - iStartPos - 50 * 2;

			menuentry_s sSettings;
			sSettings.wszText = L"Settings";
			sSettings.onClick = &MainMenu_onOpenSettings;
			sSettings.x = 10;
			sSettings.y = h - iStartPos - 50;

			menuentry_s sQuit;
			sQuit.wszText = L"Quit";
			sQuit.onClick = &MainMenu_onQuitGame;
			sQuit.x = 10;
			sQuit.y = h - iStartPos;

			this->m_vEntries.push_back(sNews);
			this->m_vEntries.push_back(sPackages);
			this->m_vEntries.push_back(sSettings);
			this->m_vEntries.push_back(sQuit);

			this->m_pFont = pRenderer->LoadFont(L"Verdana", MAINMENU_FONTSIZE_W, MAINMENU_FONTSIZE_H);
			if (!this->m_pFont) {
				return false;
			}

			return true;
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			IMenu::OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

			for (size_t i = 0; i < this->m_vEntries.size(); i++) {
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

		virtual void Draw(void)
		{
			for (size_t i = 0; i < this->m_vEntries.size(); i++) {
				if (this->m_vEntries[i].bHover) {
					pRenderer->DrawString(this->m_pFont, this->m_vEntries[i].wszText, this->m_vEntries[i].x, this->m_vEntries[i].y, 230, 230, 230, 150);
				} else {
					pRenderer->DrawString(this->m_pFont, this->m_vEntries[i].wszText, this->m_vEntries[i].x, this->m_vEntries[i].y, 200, 200, 200, 150);
				}
			}
		}

		virtual void Release(void)
		{
			
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

		bool Initialize(int w, int h)
		{
			this->m_hCursor = pRenderer->LoadSprite(wszBasePath + L"media\\menucursor.png", 1, 16, 16, 1, false);
			if (this->m_hCursor == GFX_INVALID_SPRITE_ID) {
				return false;
			}

			if (!oMainMenu.Initialize(w, h)) {
				return false;
			}

			oMainMenu.SetActiveStatus(true);

			return true;
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			if (!this->m_bOpen)
				return;

			if (!iMouseKey) {
				this->m_iMouseX = x;
				this->m_iMouseY = y;
			}

			oMainMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		void Draw(void)
		{
			if (!this->m_bOpen)
				return;
			
			pRenderer->DrawSprite(this->m_hCursor, this->m_iMouseX, this->m_iMouseY, 0, 0.0f);

			if (oMainMenu.IsActive()) {
				oMainMenu.Draw();
			}
		}

		void Release(void)
		{
			oMainMenu.Release();
		}

		void SetOpenStatus(bool status)
		{
			this->m_bOpen = status;
		}

		bool IsOpen(void)
		{
			return this->m_bOpen;
		}
	};
}