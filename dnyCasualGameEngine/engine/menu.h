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

		//Called for key events
		virtual void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
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

	class CForm : public IMenu {
	private:
		Entity::Vector m_vecPos;
		Entity::Vector m_vecSize;
		std::wstring m_wszTitle;
		Entity::Color m_sColBorder;
		Entity::Color m_sColTitleBar;
		Entity::Color m_sColTitleBg;
		Entity::Color m_sColBody;
		Entity::Color m_sColTitleText;
	public:
		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			return true;
		}

		virtual void Release(void)
		{
		}

		void Draw(void)
		{
			//Draw form

			pRenderer->DrawBox(this->m_vecPos[0], this->m_vecPos[1], this->m_vecSize[0], iDefaultFontSize[1] * 2, 1, this->m_sColBorder.r, this->m_sColBorder.g, this->m_sColBorder.b, this->m_sColBorder.a);
			pRenderer->DrawFilledBox(this->m_vecPos[0] + 1, this->m_vecPos[1] + 1, this->m_vecSize[0] - 2, iDefaultFontSize[1] * 2 - 2, this->m_sColTitleBar.r, this->m_sColTitleBar.g, this->m_sColTitleBar.b, this->m_sColTitleBar.a);
			pRenderer->DrawString(pDefaultFont, this->m_wszTitle, this->m_vecPos[0] + 10, this->m_vecPos[1] + iDefaultFontSize[1] / 2, this->m_sColTitleText.r, this->m_sColTitleText.g, this->m_sColTitleText.b, this->m_sColTitleText.a);
			pRenderer->DrawBox(this->m_vecPos[0], this->m_vecPos[1] + iDefaultFontSize[1] * 2, this->m_vecSize[0], this->m_vecSize[1] - iDefaultFontSize[1] * 2, 1, this->m_sColBorder.r, this->m_sColBorder.g, this->m_sColBorder.b, this->m_sColBorder.a);
			pRenderer->DrawFilledBox(this->m_vecPos[0] + 1, this->m_vecPos[1] + iDefaultFontSize[1] * 2 + 2, this->m_vecSize[0] - 2, this->m_vecSize[1] - iDefaultFontSize[1] * 2 - 1, this->m_sColBody.r, this->m_sColBody.g, this->m_sColBody.b, this->m_sColBody.a);
		}

		//Setters
		void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
		void SetSize(const Entity::Vector& vec) { this->m_vecSize = vec; }
		void SetTitle(const std::wstring& wszTitle) { this->m_wszTitle = wszTitle; }
		void SetBorderColor(const Entity::Color& col) { this->m_sColBorder = col; }
		void SetTitleBarColor(const Entity::Color& col) { this->m_sColTitleBar = col; }
		void SetBodyColor(const Entity::Color& col) { this->m_sColBody = col; }
		void SetTitleTextColor(const Entity::Color& col) { this->m_sColTitleText = col; }
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
			}

			this->m_oImageListView.Draw();
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

	class CTabMenu : public IMenu {
	private:
		struct menu_item_s {
			std::wstring wszTitle;
			IMenu* pMenu;
		};

		std::vector<menu_item_s> m_vItems;
		size_t m_uiSelectedTab;
		Entity::Color m_sColorText;
		Entity::Color m_sColorHover;
		Entity::Color m_sColorSelected;
		Entity::Vector m_vecPos;
		int m_iTabSpace;
		Entity::Vector m_vecMousePos;
		size_t m_uiHoverItem;
	public:
		CTabMenu() : m_uiSelectedTab(std::string::npos), m_uiHoverItem(std::string::npos), m_iTabSpace(20) {}
		~CTabMenu() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			return true;
		}

		virtual void Release(void)
		{
		}

		virtual void AddItem(const std::wstring& wszTitle, IMenu* pMenu)
		{
			//Add menu item

			menu_item_s sItem;
			sItem.wszTitle = wszTitle;
			sItem.pMenu = pMenu;

			this->m_vItems.push_back(sItem);
		}

		virtual void Draw(void)
		{
			//Draw tab menu
			
			this->m_uiHoverItem = std::string::npos;

			for (size_t i = 0; i < this->m_vItems.size(); i++) {
				int xpos = this->m_vecPos[0] + ((int)i * ((iDefaultFontSize[0] * (int)this->m_vItems[i].wszTitle.length()) + this->m_iTabSpace));
				int ypos = this->m_vecPos[1];
				
				if (this->m_uiSelectedTab == i) {
					pRenderer->DrawFilledBox(xpos - 10, ypos - 10, (int)this->m_vItems[i].wszTitle.length() * iDefaultFontSize[0] + 20, iDefaultFontSize[1] + 20, this->m_sColorSelected.r, this->m_sColorSelected.g, this->m_sColorSelected.b, this->m_sColorSelected.a);
				}

				if ((this->m_vecMousePos[0] > xpos) && (this->m_vecMousePos[0] < xpos + (int)this->m_vItems[i].wszTitle.length() * iDefaultFontSize[0]) && (this->m_vecMousePos[1] > ypos) && (this->m_vecMousePos[1] < ypos + iDefaultFontSize[1])) {
					this->m_uiHoverItem = i;

					pRenderer->DrawBox(xpos - 10, ypos - 10, (int)this->m_vItems[i].wszTitle.length() * iDefaultFontSize[0] + 20, iDefaultFontSize[1] + 20, 2, this->m_sColorHover.r, this->m_sColorHover.g, this->m_sColorHover.b, this->m_sColorHover.a);
				}

				pRenderer->DrawString(pDefaultFont, this->m_vItems[i].wszTitle, xpos, ypos, this->m_sColorText.r, this->m_sColorText.g, this->m_sColorText.b, this->m_sColorText.a);
			}

			if (this->m_uiSelectedTab != std::string::npos) {
				this->m_vItems[this->m_uiSelectedTab].pMenu->Draw();
			}
		}

		virtual void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle key events

			if (this->m_uiSelectedTab != std::string::npos) {
				this->m_vItems[this->m_uiSelectedTab].pMenu->OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			}
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);
			} else {
				if ((iMouseKey == 1) && (!bDown) && (this->m_uiHoverItem != std::string::npos)) {
					this->m_uiSelectedTab = this->m_uiHoverItem;
				}
			}

			if (this->m_uiSelectedTab != std::string::npos) {
				this->m_vItems[this->m_uiSelectedTab].pMenu->OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			}
		}

		//Setters
		virtual void SetTextColor(const Entity::Color& color) { this->m_sColorText = color; }
		virtual void SetHoverColor(const Entity::Color& color) { this->m_sColorHover = color; }
		virtual void SetSelectedColor(const Entity::Color& color) { this->m_sColorSelected = color; }
		virtual void SetPosition(const Entity::Vector& pos) { this->m_vecPos = pos; }
		virtual void SetTabSpacing(int value) { this->m_iTabSpace = value; }
	};

	class CSettingsKeys : public IMenu {
	private:
		Entity::Vector m_vecPos;
	public:
		CSettingsKeys() {}
		~CSettingsKeys() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			return true;
		}

		virtual void Draw(void)
		{
			pRenderer->DrawString(pDefaultFont, L"CSettingsKeys", this->m_vecPos[0], this->m_vecPos[1], 200, 200, 200, 150);
		}

		virtual void Release(void)
		{

		}

		virtual void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
	};

	class CSettingsGfx : public IMenu {
	private:
		Entity::Vector m_vecPos;
	public:
		CSettingsGfx() {}
		~CSettingsGfx() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			return true;
		}

		virtual void Draw(void)
		{
			pRenderer->DrawString(pDefaultFont, L"CSettingsGfx", this->m_vecPos[0], this->m_vecPos[1], 200, 200, 200, 150);
		}

		virtual void Release(void)
		{

		}

		virtual void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
	};

	class CSettingsSnd : public IMenu {
	private:
		Entity::Vector m_vecPos;
	public:
		CSettingsSnd() {}
		~CSettingsSnd() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			return true;
		}

		virtual void Draw(void)
		{
			pRenderer->DrawString(pDefaultFont, L"CSettingsSnd", this->m_vecPos[0], this->m_vecPos[1], 200, 200, 200, 150);
		}

		virtual void Release(void)
		{

		}

		virtual void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
	};

	class CSettingsMenu : public IMenu {
	private:
		CSettingsKeys m_oMenuKeys;
		CSettingsGfx m_oMenuGfx;
		CSettingsSnd m_oMenuSnd;
		CTabMenu m_oTabMenu;
	public:
		CSettingsMenu() {}
		~CSettingsMenu() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize settings menu

			this->m_oMenuKeys.SetPosition(Entity::Vector(250, 250));
			this->m_oMenuGfx.SetPosition(Entity::Vector(250, 250));
			this->m_oMenuSnd.SetPosition(Entity::Vector(250, 250));

			this->m_oTabMenu.AddItem(L"Bindings", &this->m_oMenuKeys);
			this->m_oTabMenu.AddItem(L"Graphics", &this->m_oMenuGfx);
			this->m_oTabMenu.AddItem(L"Sound   ", &this->m_oMenuSnd);

			this->m_oTabMenu.SetPosition(Entity::Vector(250, 200));
			this->m_oTabMenu.SetTabSpacing(20);
			this->m_oTabMenu.SetTextColor(Entity::Color(200, 200, 200, 150));
			this->m_oTabMenu.SetHoverColor(Entity::Color(50, 200, 0, 150));
			this->m_oTabMenu.SetSelectedColor(Entity::Color(10, 150, 0, 150));

			return true;
		}

		virtual void Draw(void)
		{
			//Draw menu

			this->m_oTabMenu.Draw();
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oTabMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle key events

			this->m_oTabMenu.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void Release(void)
		{
		}
	};

	class CCursor {
	private:
		DxRenderer::HD3DSPRITE m_hCursor;
		Entity::Vector m_vecPos;
		bool m_bActive;
	public:
		CCursor() : m_bActive(false) {}
		~CCursor() {}

		bool Initialize(void)
		{
			//Initialize cursor

			this->m_hCursor = pRenderer->LoadSprite(wszBasePath + L"media\\menucursor.png", 1, 16, 16, 1, false);
			if (this->m_hCursor == GFX_INVALID_SPRITE_ID) {
				return false;
			}

			return true;
		}

		void Release(void)
		{
			//Release resource

			pRenderer->FreeSprite(this->m_hCursor);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecPos = Entity::Vector(x, y);
			}
		}

		void Draw(void)
		{
			//Draw cursor

			if (!this->m_bActive) {
				return;
			}

			pRenderer->DrawSprite(this->m_hCursor, this->m_vecPos[0], this->m_vecPos[1], 0, 0.0f);
		}

		//Getter and setter
		bool IsActive(void) { return this->m_bActive; }
		void SetActiveStatus(bool value) { this->m_bActive = value; }
	};

	class CMenu {
	private:
		CMainMenu m_oMainMenu;
		CPackageMenu m_oPackageMenu;
		CSettingsMenu m_oSettingsMenu;
		bool m_bOpen;
		int m_iMouseX;
		int m_iMouseY;
	public:
		CMenu() : m_bOpen(false) {}
		~CMenu() {}

		bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize menu

			if (!this->m_oMainMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			if (!this->m_oPackageMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			if (!this->m_oSettingsMenu.Initialize(w, h, pGameStarted)) {
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

			if (this->m_oSettingsMenu.IsActive()) {
				this->m_oSettingsMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
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

			if (this->m_oSettingsMenu.IsActive()) {
				this->m_oSettingsMenu.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			}
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
				} else if (this->m_oSettingsMenu.IsActive()) {
					this->m_oSettingsMenu.Draw();
				}
			}
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
			this->m_oSettingsMenu.SetActiveStatus(false);
		}

		void OpenSettingsMenu(void)
		{
			this->m_oSettingsMenu.SetActiveStatus(true);
			this->m_oPackageMenu.SetActiveStatus(false);
		}

		void OnCloseAll(void)
		{
			this->m_oPackageMenu.SetActiveStatus(false);
			this->m_oSettingsMenu.SetActiveStatus(false);
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

	class CIntermissionMenu : public IButtonClickHandler {
	private:
		CForm m_oForm;
		CButton m_oButton;
		int m_iScore;
		std::wstring m_wszTitle;
		bool m_bGameFinished;
		Entity::Vector m_vecPos;
		Entity::Vector m_vecSize;
	public:
		CIntermissionMenu() : m_bGameFinished(false) {}
		~CIntermissionMenu() {}

		bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize dialog

			this->m_vecSize = Entity::Vector(w, h);

			if (!this->m_oForm.Initialize(w, h, pGameStarted)) {
				return false;
			}

			this->m_oForm.SetSize(this->m_vecSize);
			this->m_oForm.SetBodyColor(Entity::Color(150, 150, 150, 150));
			this->m_oForm.SetBorderColor(Entity::Color(200, 200, 200, 150));
			this->m_oForm.SetTitleBarColor(Entity::Color(50, 125, 0, 150));
			this->m_oForm.SetTitleTextColor(Entity::Color(250, 250, 250, 150));

			this->m_oButton.SetFrameColor(Entity::Color(200, 200, 200, 150));
			this->m_oButton.SetFillColor(Entity::Color(50, 150, 50, 150));
			this->m_oButton.SetHoverColor(Entity::Color(70, 180, 70, 150));
			this->m_oButton.SetTextColor(Entity::Color(250, 250, 250, 150));
			this->m_oButton.SetSize(Entity::Vector(300, 35));
			this->m_oButton.SetOwner(this);

			return true;
		}

		void SetGameFinishState(bool value) 
		{ 
			//Set game state value

			this->m_bGameFinished = value; 

			if (value) {
				this->m_oButton.SetText(L"Return to main menu");
				this->m_oForm.SetTitle(L"Game completed!");
			} else {
				this->m_oButton.SetText(L"Proceed with next map");
				this->m_oForm.SetTitle(L"Map completed!");
			}
		}

		void SetGameScore(int value)
		{
			//Set game score value

			this->m_iScore = value;
		}

		void SetPosition(const Entity::Vector& vec)
		{
			//Set position

			this->m_vecPos = vec;

			this->m_oForm.SetPosition(vec);
			this->m_oButton.SetPosition(Entity::Vector(vec[0] + 10, vec[1] + this->m_vecSize[1] - 45));
		}

		void Draw(void)
		{
			//Draw menu

			this->m_oForm.Draw();
			this->m_oButton.Draw();

			pRenderer->DrawString(pDefaultFont, L"Your achieved score: " + std::to_wstring(this->m_iScore), this->m_vecPos[0] + 20, this->m_vecPos[1] + 100, 200, 200, 200, 150);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oButton.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void OnButtonClick(class CButton* pButton);
	};
}