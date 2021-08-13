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
#include "entity.h"

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

	class IButtonClickHandler {
	public:
		virtual void OnButtonClick(class CButton* pButton) = 0;
	};
	class CButton {
	private:
		IButtonClickHandler* m_pOwner;
		std::wstring m_wszText;
		Entity::Color m_sTextColor;
		Entity::Color m_sFrameColor;
		Entity::Color m_sFillColor;
		Entity::Color m_sHoverColor;
		Entity::Vector m_vecPos;
		Entity::Vector m_vecSize;
		bool m_bHover;
		Entity::Vector m_vecMousePos;
	public:
		CButton() : m_bHover(false), m_pOwner(nullptr) {}
		~CButton() {}

		void Draw(void)
		{
			//Draw button

			this->m_bHover = false;
			if ((this->m_vecMousePos[0] > this->m_vecPos[0]) && (this->m_vecMousePos[0] < this->m_vecPos[0] + this->m_vecSize[0]) && (this->m_vecMousePos[1] > this->m_vecPos[1]) && (this->m_vecMousePos[1] < this->m_vecPos[1] + this->m_vecSize[1])) {
				this->m_bHover = true;
			}

			pRenderer->DrawBox(this->m_vecPos[0], this->m_vecPos[1], this->m_vecSize[0], this->m_vecSize[1], 2, this->m_sFrameColor.r, this->m_sFrameColor.g, this->m_sFrameColor.b, this->m_sFrameColor.a);

			if (this->m_bHover) {
				pRenderer->DrawFilledBox(this->m_vecPos[0] + 1, this->m_vecPos[1] + 1, this->m_vecSize[0], this->m_vecSize[1] - 1, this->m_sHoverColor.r, this->m_sHoverColor.g, this->m_sHoverColor.b, this->m_sHoverColor.a);
			}
			else {
				pRenderer->DrawFilledBox(this->m_vecPos[0] + 1, this->m_vecPos[1] + 1, this->m_vecSize[0], this->m_vecSize[1] - 1, this->m_sFillColor.r, this->m_sFillColor.g, this->m_sFillColor.b, this->m_sFillColor.a);
			}

			pRenderer->DrawString(pDefaultFont, this->m_wszText, this->m_vecPos[0] + this->m_vecSize[0] / 2 - (iDefaultFontSize[0] * (int)this->m_wszText.length()) / 2, this->m_vecPos[1] + this->m_vecSize[1] / 2 - iDefaultFontSize[1] / 2, this->m_sTextColor.r, this->m_sTextColor.g, this->m_sTextColor.b, this->m_sTextColor.a);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);
			} else {
				if ((iMouseKey == 1) && (!bDown) && (this->m_bHover)) {
					this->m_pOwner->OnButtonClick(this);
				}
			}
		}

		//Setters
		void SetOwner(IButtonClickHandler* pOwner) { this->m_pOwner = pOwner; }
		void SetText(const std::wstring& wszText) { this->m_wszText = wszText; }
		void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
		void SetSize(const Entity::Vector& vec) { this->m_vecSize = vec; }
		void SetTextColor(const Entity::Color& color) { this->m_sTextColor = color; }
		void SetFrameColor(const Entity::Color& color) { this->m_sFrameColor = color; }
		void SetFillColor(const Entity::Color& color) { this->m_sFillColor = color; }
		void SetHoverColor(const Entity::Color& color) { this->m_sHoverColor = color; }
	};

	class IImageListViewSelector {
	public:
		virtual void OnImageSelected(class CImageListView* pImageListView, size_t uiItemId) = 0;
	};
	class CImageListView {
	public:
		struct viewitem_s {
			DxRenderer::HD3DSPRITE hSprite;
			std::wstring wszIdent;
		};
	private:
		IImageListViewSelector* m_pOwner;
		Entity::Vector m_vecPos;
		Entity::Vector m_vecSize;
		Entity::Vector m_vecImageSize;
		Entity::Color m_sHoverColor;
		Entity::Color m_sSelectColor;
		bool m_bHover;
		Entity::Vector m_vecMousePos;
		std::vector<viewitem_s> m_vItems;
		size_t m_uiItemDisplayCountX;
		size_t m_uiItemDisplayCountY;
		int m_iImageGap;
		size_t m_uiDrawingIndex;
		size_t m_uiSelectedItem;
		size_t m_uiHoverItem;
	public:
		CImageListView() : m_bHover(false), m_pOwner(nullptr), m_uiDrawingIndex(0), m_uiSelectedItem(0) {}
		~CImageListView() {}

		void AddItem(DxRenderer::HD3DSPRITE hSprite, const std::wstring& wszIdent)
		{
			//Add item to list

			viewitem_s sItem;
			sItem.hSprite = hSprite;
			sItem.wszIdent = wszIdent;

			this->m_vItems.push_back(sItem);
		}

		void Draw(void)
		{
			//Draw list view
			
			this->m_uiHoverItem = std::string::npos;

			size_t uiLoopMax = (this->m_uiDrawingIndex < this->m_vItems.size() - this->m_uiItemDisplayCountX * this->m_uiItemDisplayCountY) ? this->m_uiDrawingIndex + this->m_uiItemDisplayCountX * this->m_uiItemDisplayCountY : this->m_vItems.size();

			for (size_t i = this->m_uiDrawingIndex; i < uiLoopMax; i++) {
				int countx = (int)((i - this->m_uiDrawingIndex) % this->m_uiItemDisplayCountX);
				int county = (int)((i - this->m_uiDrawingIndex) / this->m_uiItemDisplayCountY);
				int xpos = this->m_vecPos[0] + (countx * (this->m_iImageGap * 2 + this->m_vecImageSize[0]));
				int ypos = this->m_vecPos[1] + (county * (this->m_iImageGap * 2 + this->m_vecImageSize[1]));

				this->m_bHover = false;
				if ((this->m_vecMousePos[0] > xpos) && (this->m_vecMousePos[0] < xpos + this->m_vecImageSize[0]) && (this->m_vecMousePos[1] > ypos) && (this->m_vecMousePos[1] < ypos + this->m_vecImageSize[1])) {
					this->m_bHover = true;
					this->m_uiHoverItem = i;
				}

				if (this->m_bHover) {
					pRenderer->DrawBox(xpos - 1, ypos - 1, this->m_vecImageSize[0] + 1, this->m_vecImageSize[1] + 1, 1, this->m_sHoverColor.r, this->m_sHoverColor.g, this->m_sHoverColor.b, this->m_sHoverColor.a);
				}

				if (this->m_uiSelectedItem == i) {
					pRenderer->DrawBox(xpos - 1, ypos - 1, this->m_vecImageSize[0] + 1, this->m_vecImageSize[1] + 1, 1, this->m_sSelectColor.r, this->m_sSelectColor.g, this->m_sSelectColor.b, this->m_sSelectColor.a);
				}


				pRenderer->DrawSprite(this->m_vItems[i].hSprite, xpos, ypos, 0, 0.0f);
			}
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);
			} else {
				if ((iMouseKey == 1) && (!bDown) && (this->m_uiHoverItem != std::string::npos)) {
					this->m_uiSelectedItem = this->m_uiHoverItem;
					this->m_pOwner->OnImageSelected(this, this->m_uiSelectedItem);
				}
			}
		}

		void OnMouseWheelEvent(short wDistance, bool bForward)
		{
			//Handle mouse wheel event
			
			if (!bForward) {
				if (this->m_uiDrawingIndex < this->m_vItems.size() - this->m_uiItemDisplayCountX * this->m_uiItemDisplayCountY) {
					this->m_uiDrawingIndex++;
				}
			} else {
				if (this->m_uiDrawingIndex > 0) {
					this->m_uiDrawingIndex--;
				}
			}
		}

		void UpdateDimensions(void)
		{
			//Update drawing dimension information

			this->m_uiItemDisplayCountX = this->m_vecSize[0] / (this->m_vecImageSize[0] + this->m_iImageGap * 2);
			this->m_uiItemDisplayCountY = this->m_vecSize[1] / (this->m_vecImageSize[1] + this->m_iImageGap * 2);
		}

		//Setters
		void SetOwner(IImageListViewSelector* pOwner) { this->m_pOwner = pOwner; }
		void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
		void SetSize(const Entity::Vector& vec) { this->m_vecSize = vec;  }
		void SetImageSize(const Entity::Vector& vec) { this->m_vecImageSize = vec; }
		void SetImageGap(int val) { this->m_iImageGap = val; }
		void SetHoverColor(const Entity::Color& color) { this->m_sHoverColor = color; }
		void SetSelectColor(const Entity::Color& color) { this->m_sSelectColor = color; }

		//Getters
		const size_t GetSelection(void) { return this->m_uiSelectedItem; }
		const viewitem_s& GetItem(const size_t uiIdent) const { static viewitem_s sDummy;  if (uiIdent < this->m_vItems.size()) return this->m_vItems[uiIdent]; else return sDummy; }
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

	class CPackageMenu : public IMenu, public IButtonClickHandler, IImageListViewSelector {
	private:
		struct package_s {
			std::wstring wszIdent;
			DxRenderer::HD3DSPRITE hPreview;
			std::vector<std::wstring> vAboutContent;
		};

		std::vector<package_s> m_vPackages;
		size_t m_uiSelectedPackage;
		CButton m_oButton;
		CImageListView m_oImageListView;
	public:
		CPackageMenu() : m_uiSelectedPackage(std::string::npos) {}
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

						this->m_oImageListView.AddItem(sPackage.hPreview, sPackage.wszIdent);
					}
				}

				FindClose(hFileSearch);
			}
			
			//Select first entry
			if (this->m_vPackages.size() > 0) {
				this->m_uiSelectedPackage = 0;
			}

			this->m_oButton.SetText(L"Play!");
			this->m_oButton.SetPosition(Entity::Vector(250, 200 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1]));
			this->m_oButton.SetSize(Entity::Vector(130, 35));
			this->m_oButton.SetOwner(this);
			this->m_oButton.SetTextColor(Entity::Color(250, 250, 250, 150));
			this->m_oButton.SetFrameColor(Entity::Color(0, 0, 0, 150));
			this->m_oButton.SetFillColor(Entity::Color(103, 225, 123, 150));
			this->m_oButton.SetHoverColor(Entity::Color(143, 235, 155, 150));

			this->m_oImageListView.SetOwner(this);
			this->m_oImageListView.SetPosition(Entity::Vector(250, 200 + 45 + (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1]));
			this->m_oImageListView.SetSize(Entity::Vector(700, 400));
			this->m_oImageListView.SetImageGap(10);
			this->m_oImageListView.SetImageSize(Entity::Vector(195, 90));
			this->m_oImageListView.SetHoverColor(Entity::Color(0, 150, 0, 150));
			this->m_oImageListView.SetSelectColor(Entity::Color(0, 200, 0, 150));
			this->m_oImageListView.UpdateDimensions();
			
			return true;
		}

		virtual void Draw(void)
		{
			//Draw menu

			if (!this->m_bActive)
				return;

			const int iStartPosX = 250;
			const int iStartPosY = 200;

			//Draw selected package preview
			if ((this->m_uiSelectedPackage != std::string::npos) && (this->m_uiSelectedPackage < this->m_vPackages.size())) {
				//Draw about content
				for (size_t i = 0; i < this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size(); i++) {
					pRenderer->DrawString(pDefaultFont, this->m_vPackages[this->m_uiSelectedPackage].vAboutContent[i], iStartPosX, iStartPosY + (int)i * iDefaultFontSize[1], 200, 200, 200, 150);
				}

				this->m_oButton.Draw();
				this->m_oImageListView.Draw();
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

		//Called for mouse wheel events
		virtual void OnMouseWheelEvent(short wDistance, bool bForward)
		{
			this->m_oImageListView.OnMouseWheelEvent(wDistance, bForward);
		}

		//Called for button click events
		virtual void OnButtonClick(class CButton* pButton);

		//Called for image list view selection events
		virtual void OnImageSelected(CImageListView* pImageListView, size_t uiItemId);

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
			
			if (this->m_oPackageMenu.IsActive()) {
				this->m_oPackageMenu.OnMouseWheelEvent(wDistance, bForward);
			}
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