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
	void MainMenu_OnResumeGame(class CMenu* pMenu);
	void MainMenu_OnStopGame(class CMenu* pMenu);
	void MainMenu_OnOpenNews(class CMenu* pMenu);
	void MainMenu_OnOpenPackages(class CMenu* pMenu);
	void MainMenu_OnOpenSettings(class CMenu* pMenu);
	void MainMenu_OnQuitGame(class CMenu* pMenu);

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
		typedef void (*TOnClick)(class CMenu* pMenu);

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
		class CMenu* m_pContainer;
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
							this->m_vEntries[i].onClick(this->m_pContainer);
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

		virtual void SetClassPointer(class CMenu* pContainer)
		{
			//Set menu container class instance pointer

			this->m_pContainer = pContainer;
		}
	};

	class CPackageMenu : public IMenu {
	private:
		struct package_s {
			std::wstring wszIdent;
			DxRenderer::HD3DSPRITE hPreview;
			std::vector<std::wstring> vAboutContent;
		};

		std::vector<package_s> m_vPackages;
		size_t m_uiSelectedPackage;
		bool m_bPlayButtonHover;
	public:
		CPackageMenu() : m_uiSelectedPackage(std::string::npos), m_bPlayButtonHover(false) {}
		~CPackageMenu() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize packages

			WIN32_FIND_DATA sFindData = { 0 };

			//Read base data for each available package
			HANDLE hFileSearch = FindFirstFile((wszBasePath + L"packages\\*.*").c_str(), &sFindData);
			if (hFileSearch != INVALID_HANDLE_VALUE) {
				while (FindNextFile(hFileSearch, &sFindData)) {
					if (sFindData.cFileName[0] == '.') {
						continue;
					}

					if ((sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
						package_s sPackage;

						sPackage.wszIdent = sFindData.cFileName;
						sPackage.vAboutContent = Utils::ReadFileLines(wszBasePath + L"packages\\" + sPackage.wszIdent + L"\\about.txt");
						sPackage.hPreview = pRenderer->LoadSprite(wszBasePath + L"packages\\" + sPackage.wszIdent + L"\\preview.png", 1, 195, 90, 1, true);
						
						this->m_vPackages.push_back(sPackage);
					}
				}

				FindClose(hFileSearch);
			}
			
			//Select first entry
			if (this->m_vPackages.size() > 0) {
				this->m_uiSelectedPackage = 0;
			}
			
			return true;
		}

		virtual void Draw(void)
		{
			//Draw menu

			if (!this->m_bActive)
				return;

			const int iStartPosX = 250;
			const int iStartPosY = 200;
			const int iButtonWidth = 130;
			const int iButtonHeight = 35;

			//Draw selected package preview
			if ((this->m_uiSelectedPackage != std::string::npos) && (this->m_uiSelectedPackage < this->m_vPackages.size())) {
				//Draw about content
				for (size_t i = 0; i < this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size(); i++) {
					pRenderer->DrawString(pDefaultFont, this->m_vPackages[this->m_uiSelectedPackage].vAboutContent[i], iStartPosX, iStartPosY + (int)i * iDefaultFontSize[1], 200, 200, 200, 150);
				}

				this->m_bPlayButtonHover = false;
				if ((this->m_iMouseX > iStartPosX + 1) && (this->m_iMouseX < iStartPosX + 1 + iButtonWidth) && (this->m_iMouseY > iStartPosY + 1 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1]) && (this->m_iMouseY < iStartPosY + 1 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1] + iButtonHeight)) {
					this->m_bPlayButtonHover = true;
				}

				//Draw play button
				pRenderer->DrawBox(iStartPosX, iStartPosY + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1], iButtonWidth, iButtonHeight, 2, 0, 0, 0, 150);
				if (this->m_bPlayButtonHover) {
					pRenderer->DrawFilledBox(iStartPosX + 1, iStartPosY + 1 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1], iButtonWidth, iButtonHeight - 1, 143, 235, 155, 150);
				} else {
					pRenderer->DrawFilledBox(iStartPosX + 1, iStartPosY + 1 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1], iButtonWidth, iButtonHeight - 1, 103, 225, 123, 150);
				}
				pRenderer->DrawString(pDefaultFont, L"Play!", iStartPosX + iButtonWidth / 2 - iDefaultFontSize[0] * 5 / 2, iStartPosY + iButtonHeight / 2 - iDefaultFontSize[1] / 2 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1], 250, 250, 250, 150);
			}
		}

		virtual void Release(void)
		{
			//Release menu

			for (size_t i = 0; i < this->m_vPackages.size(); i++) {
				pRenderer->FreeSprite(this->m_vPackages[i].hPreview);
			}

			this->m_vPackages.clear();
		}

		//Called for mouse events
		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);

		virtual void SetActiveStatus(bool status)
		{
			this->m_bActive = status;
		}

		virtual bool IsActive(void)
		{
			return this->m_bActive;
		}
	};

	class CMenu {
	private:
		CMainMenu m_oMainMenu;
		CPackageMenu m_oPackageMenu;
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

			if (!this->m_oMainMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			if (!this->m_oPackageMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			this->m_oMainMenu.SetClassPointer(this);
			this->m_oMainMenu.SetActiveStatus(true);

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

			if (this->m_oMainMenu.IsActive()) {
				this->m_oMainMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			}

			if (this->m_oPackageMenu.IsActive()) {
				this->m_oPackageMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			}
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

			if (this->m_oMainMenu.IsActive()) {
				this->m_oMainMenu.Draw();

				if (this->m_oPackageMenu.IsActive()) {
					this->m_oPackageMenu.Draw();
				}
			}

			pRenderer->DrawSprite(this->m_hCursor, this->m_iMouseX, this->m_iMouseY, 0, 0.0f);
		}

		void Release(void)
		{
			//Release menu

			this->m_oMainMenu.Release();
		}

		void SetOpenStatus(bool status)
		{
			this->m_bOpen = status;

			if (status) {
				this->m_oMainMenu.OnToggleGameActiveMenuItems();
			}
		}

		void OpenPackageMenu(void)
		{
			this->m_oPackageMenu.SetActiveStatus(true);
		}

		void OnStopGame(void)
		{
			this->m_oMainMenu.OnToggleGameActiveMenuItems();
		}

		bool IsOpen(void)
		{
			return this->m_bOpen;
		}
	};
}