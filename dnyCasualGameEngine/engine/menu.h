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
	void MainMenu_OnOpenPlay(class CMenu* pMenu);
	void MainMenu_OnOpenPackages(class CMenu* pMenu);
	void MainMenu_OnOpenSettings(class CMenu* pMenu);
	void MainMenu_OnQuitGame(class CMenu* pMenu);

	/* Base menu interface */
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

	/* Button component */

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

	/* Checkbox component */
	const int CB_BOX_SIZE = 12;
	const int CB_BOX_GAP = 10;
	class CCheckbox {
	private:
		bool m_bValue;
		bool m_bHover;
		std::wstring m_wszLabel;
		Entity::Vector m_vecPosition;
		Entity::Vector m_vecMousePos;
		Entity::Color m_sFrameColor;
		Entity::Color m_sHoverColor;
		Entity::Color m_sCheckColor;
		Entity::Color m_sLabelColor;

		int GetComponentWidth(void)
		{
			//Return width of component

			return CB_BOX_SIZE + 2 + CB_BOX_GAP + (int)this->m_wszLabel.length() * iDefaultFontSize[0];
		}

		bool IsInsideComponent(void)
		{
			//Check if inside component

			if ((this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->GetComponentWidth()) && (this->m_vecMousePos[1] > this->m_vecPosition[1]) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + iDefaultFontSize[1])) {
				return true;
			}

			return false;
		}
	public:
		CCheckbox() : m_bValue(false), m_bHover(false) {}
		~CCheckbox() {}

		void Draw(void)
		{
			//Draw component

			Entity::Color sFrameColor;

			if (this->m_bHover) {
				sFrameColor = this->m_sHoverColor;
			} else {
				sFrameColor = this->m_sFrameColor;
			}

			pRenderer->DrawBox(this->m_vecPosition[0], this->m_vecPosition[1], CB_BOX_SIZE + 1, CB_BOX_SIZE + 1, 1, sFrameColor.r, sFrameColor.g, sFrameColor.b, sFrameColor.a);

			if (this->m_bValue) {
				pRenderer->DrawFilledBox(this->m_vecPosition[0] + 1, this->m_vecPosition[1] + 1, CB_BOX_SIZE, CB_BOX_SIZE, this->m_sCheckColor.r, this->m_sCheckColor.g, this->m_sCheckColor.b, this->m_sCheckColor.a);
			}

			pRenderer->DrawString(pDefaultFont, this->m_wszLabel, this->m_vecPosition[0] + 2 + CB_BOX_SIZE + CB_BOX_GAP, this->m_vecPosition[1], this->m_sLabelColor.r, this->m_sLabelColor.g, this->m_sLabelColor.b, this->m_sLabelColor.a);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);

				this->m_bHover = this->IsInsideComponent();
			} else {
				if ((iMouseKey == 1) && (!bDown)) {
					if (this->IsInsideComponent()) {
						this->m_bValue = !this->m_bValue;
					}
				}
			}
		}

		//Setters
		void SetLabel(const std::wstring& wszLabel) { this->m_wszLabel = wszLabel; }
		void SetChecked(bool bCheckValue) { this->m_bValue = bCheckValue; }
		void SetPosition(const Entity::Vector& vecPos) { this->m_vecPosition = vecPos;  }
		void SetFrameColor(const Entity::Color& col) { this->m_sFrameColor = col; }
		void SetHoverColor(const Entity::Color& col) { this->m_sHoverColor = col; }
		void SetCheckColor(const Entity::Color& col) { this->m_sCheckColor = col; }
		void SetLabelColor(const Entity::Color& col) { this->m_sLabelColor = col; }

		//Getters
		bool IsChecked(void) const { return this->m_bValue; }
		std::wstring GetLabel(void) { return this->m_wszLabel; }
	};

	/* Combobox component */
	const int CB_HEIGHT = 20;
	const int CB_LIST_ITEM_GAP = 2;
	class CComboBox {
	private:
		bool m_bHover;
		bool m_bOpen;
		std::vector<std::wstring> m_vItems;
		Entity::Vector m_vecPosition;
		int m_iWidth;
		Entity::Vector m_vecMousePos;
		size_t m_uiSelectedItem;
		Entity::Color m_sFrameColor;
		Entity::Color m_sTextColor;
		Entity::Color m_sHoverColor;
		Entity::Color m_sFillColor;

		void SetValueFromPosition(void)
		{
			//Set value from position

			if (this->m_bOpen) {
				for (size_t i = 0; i < this->m_vItems.size(); i++) {
					if ((this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->m_iWidth) && (this->m_vecMousePos[1] > this->m_vecPosition[1] + CB_HEIGHT + 1 + ((int)i * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP))) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + CB_HEIGHT + 1 + ((int)i * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP)) + iDefaultFontSize[1])) {
						this->m_uiSelectedItem = i;
						break;
					}
				}
			}
		}

		bool IsInsideComponent(void)
		{
			//Check if inside component

			if (this->m_bOpen) {
				return (this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->m_iWidth) && (this->m_vecMousePos[1] > this->m_vecPosition[1]) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + CB_HEIGHT + 1 + 1 + ((int)this->m_vItems.size() * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP)));
			} else {
				return (this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->m_iWidth) && (this->m_vecMousePos[1] > this->m_vecPosition[1]) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + CB_HEIGHT + 1);
			}

			return false;
		}
	public:
		CComboBox() : m_bHover(false), m_bOpen(false), m_iWidth(150) {}
		~CComboBox() {}

		void Draw(void)
		{
			//Draw component

			pRenderer->DrawBox(this->m_vecPosition[0], this->m_vecPosition[1], this->m_iWidth + 1, CB_HEIGHT + 1, 1, this->m_sFrameColor.r, this->m_sFrameColor.g, this->m_sFrameColor.b, this->m_sFrameColor.a);
			pRenderer->DrawFilledBox(this->m_vecPosition[0] + 1, this->m_vecPosition[1] + 1, this->m_iWidth, CB_HEIGHT, this->m_sFillColor.r, this->m_sFillColor.g, this->m_sFillColor.b, this->m_sFillColor.a);
			pRenderer->DrawString(pDefaultFont, this->m_vItems[this->m_uiSelectedItem], this->m_vecPosition[0] + 5, this->m_vecPosition[1] + CB_HEIGHT / 2 - iDefaultFontSize[1] / 2, this->m_sTextColor.r, this->m_sTextColor.g, this->m_sTextColor.b, this->m_sTextColor.a);

			if (this->m_bOpen) {
				pRenderer->DrawBox(this->m_vecPosition[0], this->m_vecPosition[1] + CB_HEIGHT, this->m_iWidth + 1, (int)this->m_vItems.size() * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP) + 1, 1, this->m_sFrameColor.r, this->m_sFrameColor.g, this->m_sFrameColor.b, this->m_sFrameColor.a);
				pRenderer->DrawFilledBox(this->m_vecPosition[0], this->m_vecPosition[1] + CB_HEIGHT + 1, this->m_iWidth, (int)this->m_vItems.size() * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP), this->m_sFillColor.r, this->m_sFillColor.g, this->m_sFillColor.b, this->m_sFillColor.a);

				for (size_t i = 0; i < this->m_vItems.size(); i++) {
					if ((this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->m_iWidth) && (this->m_vecMousePos[1] > this->m_vecPosition[1] + CB_HEIGHT + 1 + ((int)i * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP))) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + CB_HEIGHT + 1 + ((int)i * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP)) + iDefaultFontSize[1])) {
						pRenderer->DrawFilledBox(this->m_vecPosition[0] + 1, this->m_vecPosition[1] + CB_HEIGHT + 1 + (int)i * (iDefaultFontSize[1] + CB_LIST_ITEM_GAP), this->m_iWidth, iDefaultFontSize[1], this->m_sHoverColor.r, this->m_sHoverColor.g, this->m_sHoverColor.b, this->m_sHoverColor.a);
					}

					pRenderer->DrawString(pDefaultFont, this->m_vItems[i], this->m_vecPosition[0] + 5, this->m_vecPosition[1] + 1 + CB_HEIGHT + 1 + ((int)i * iDefaultFontSize[1] + CB_LIST_ITEM_GAP), this->m_sTextColor.r, this->m_sTextColor.g, this->m_sTextColor.b, this->m_sTextColor.a);
				}
			}
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);

				this->m_bHover = this->IsInsideComponent();
			} else {
				if ((iMouseKey == 1) && (!bDown)) {
					if (this->IsInsideComponent()) {
						this->SetValueFromPosition();

						this->m_bOpen = !this->m_bOpen;
					}
				}
			}
		}

		//Setters
		void AddItem(const std::wstring& wszItem) { this->m_vItems.push_back(wszItem); }
		void RemoveItem(const size_t uiIndex) { this->m_vItems.erase(this->m_vItems.begin() + uiIndex); }
		void SetPosition(const Entity::Vector& vec) { this->m_vecPosition = vec; }
		void SetWidth(int width) { this->m_iWidth = width; }
		void SetSelectedItem(const size_t uiIndex) { this->m_uiSelectedItem = uiIndex; }
		void SetFrameColor(const Entity::Color& col) { this->m_sFrameColor = col; }
		void SetTextColor(const Entity::Color& col) { this->m_sTextColor = col; }
		void SetHoverColor(const Entity::Color& col) { this->m_sHoverColor = col; }
		void SetFillColor(const Entity::Color& col) { this->m_sFillColor = col; }

		//Getters
		std::wstring GetSelectedItem(void) { return this->m_vItems[this->m_uiSelectedItem]; }
		size_t GetItemCount(void) { return this->m_vItems.size(); }
		std::wstring GetInsertedItem(const size_t uiIndex) { return this->m_vItems[uiIndex]; }
		bool IsOpen(void) { return this->m_bOpen; }
	};

	/* Slider component */
	const int SLIDER_WIDTH = 6;
	const int SLIDER_HEIGHT = 15;
	const int SLIDER_LINE_HEIGHT = 4;
	class CSlider {
	private:
		Entity::Vector m_vecPosition;
		Entity::Vector m_vecMousePos;
		int m_iWidth;
		int m_iValue;
		int m_iMax;
		Entity::Color m_sLineColor;
		Entity::Color m_sSliderColor;
		Entity::Color m_sHoverColor;
		Entity::Color m_sHintColor;
		bool m_bHover;

		int GetSliderDrawingX(void)
		{
			//Get slider drawing X pos by value

			int iValPercent = this->m_iValue * 100 / this->m_iMax;
			int iPixelVal = (int)((float)iValPercent / 100.0 * (float)this->m_iWidth);

			return iPixelVal - SLIDER_WIDTH / 2;
		}

		void SetValueFromPosition(void)
		{
			//Set new value from position

			int iRelValue = this->m_vecMousePos[0] - this->m_vecPosition[0];
			int iRelPercent = iRelValue * 100 / this->m_iWidth;
			int iNewValue = (int)((float)iRelPercent / 100.0f * (float)this->m_iMax);

			this->m_iValue = iNewValue;
		}

		bool IsInsideComponent(void)
		{
			//Indicate if inside component

			if ((this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->m_iWidth) && (this->m_vecMousePos[1] > this->m_vecPosition[1]) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + SLIDER_LINE_HEIGHT)) {
				return true;
			}

			return false;
		}
	public:
		CSlider() : m_iMax(100), m_iWidth(150), m_bHover(false) {}
		~CSlider() {}

		void Draw(void)
		{
			//Draw component

			Entity::Color sLineColor;
			if (this->m_bHover) {
				sLineColor = this->m_sHoverColor;
			} else {
				sLineColor = this->m_sLineColor;
			}

			pRenderer->DrawFilledBox(this->m_vecPosition[0], this->m_vecPosition[1], this->m_iWidth, SLIDER_LINE_HEIGHT, sLineColor.r, sLineColor.g, sLineColor.b, sLineColor.a);
			pRenderer->DrawFilledBox(this->m_vecPosition[0] + this->GetSliderDrawingX(), this->m_vecPosition[1] - SLIDER_HEIGHT / 2, SLIDER_WIDTH, SLIDER_HEIGHT, this->m_sSliderColor.r, this->m_sSliderColor.g, this->m_sSliderColor.b, this->m_sSliderColor.a);
			pRenderer->DrawString(pDefaultFont, std::to_wstring(this->m_iValue), this->m_vecPosition[0] + this->GetSliderDrawingX(), this->m_vecPosition[1] - SLIDER_HEIGHT / 2 - iDefaultFontSize[1], this->m_sHintColor.r, this->m_sHintColor.g, this->m_sHintColor.b, this->m_sHintColor.a);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);

				this->m_bHover = this->IsInsideComponent();
			} else {
				if ((iMouseKey == 1) && (!bDown)) {
					if (this->IsInsideComponent()) {
						this->SetValueFromPosition();
					}
				}
			}
		}

		//Setters
		void SetMaxValue(int value) { this->m_iMax = value; }
		void SetValue(int value) { this->m_iValue = value; }
		void SetPosition(const Entity::Vector& vec) { this->m_vecPosition = vec; }
		void SetWidth(int width) { this->m_iWidth = width; }
		void SetLineColor(const Entity::Color& col) { this->m_sLineColor = col; }
		void SetSliderColor(const Entity::Color& col) { this->m_sSliderColor = col; }
		void SetHintColor(const Entity::Color& col) { this->m_sHintColor = col; }
		void SetHoverColor(const Entity::Color& col) { this->m_sHoverColor = col; }

		//Getters
		int GetValue(void) const { return this->m_iValue; }
		int GetMaxValue(void) const { return this->m_iMax; }
	};

	/* Form component */
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

	/* Image listview component */

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

			for (size_t i = this->m_uiDrawingIndex; i < this->m_uiDrawingIndex + this->m_uiItemDisplayCountX * this->m_uiItemDisplayCountY; i++) {
				if (i >= this->m_vItems.size()) {
					continue;
				}

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
				this->ScrollUp();
			} else {
				this->ScrollDown();
			}
		}

		void ScrollUp(void)
		{
			//Scroll up

			int iTopValue = (int)this->m_vItems.size() - (int)this->m_uiItemDisplayCountX * (int)this->m_uiItemDisplayCountY;
			if (iTopValue < 0) {
				iTopValue = 0;
			}

			if (this->m_uiDrawingIndex < (size_t)iTopValue) {
				this->m_uiDrawingIndex++;
			}
		}

		void ScrollDown(void)
		{
			//Scroll down

			if (this->m_uiDrawingIndex > 0) {
				this->m_uiDrawingIndex--;
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

	/* Main menu component */
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

			menuentry_s sPlay;
			sPlay.wszText = L"Play";
			sPlay.onClick = &MainMenu_OnOpenPlay;
			sPlay.x = 10;
			sPlay.y = h - iStartPos - 50 * 3;
			sPlay.bShow = true;

			menuentry_s sPackages;
			sPackages.wszText = L"Mods";
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
			this->m_vEntries.push_back(sPlay);
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
						pRenderer->DrawString(this->m_pFont, this->m_vEntries[i].wszText, this->m_vEntries[i].x, this->m_vEntries[i].y, 230, 230, 230, 255);
					}
					else {
						pRenderer->DrawString(this->m_pFont, this->m_vEntries[i].wszText, this->m_vEntries[i].x, this->m_vEntries[i].y, 200, 200, 200, 255);
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

	/* Listview component */
	const int LB_ITEM_GAP = 4;
	class IListBoxSelectHandler {
	public:
		virtual void OnListBoxSelectEvent(class CListBox* pListBox, size_t uiItem) = 0;
	};
	class CListBox {
	private:
		struct listbox_item_s {
			std::wstring wszDisplayText;
			std::wstring wszDataContent;
		};

		IListBoxSelectHandler* m_pHandler;
		std::vector<listbox_item_s> m_vItems;
		size_t m_uiSelectedItem;
		size_t m_uiDrawingIndex;
		size_t m_uiItemDisplayCountY;
		Entity::Vector m_vecMousePos;
		Entity::Vector m_vecPosition;
		Entity::Vector m_vecSize;
		Entity::Color m_sFrameColor;
		Entity::Color m_sFillColor;
		Entity::Color m_sTextColor;
		Entity::Color m_sHoverColor;
		Entity::Color m_sSelectColor;

		bool IsInItemRange(const size_t uiItem)
		{
			//Check if mouse coords are in item range

			if ((this->m_vecMousePos[0] > this->m_vecPosition[0]) && (this->m_vecMousePos[0] < this->m_vecPosition[0] + this->m_vecSize[0]) && (this->m_vecMousePos[1] > this->m_vecPosition[1] + 3 + (int)(uiItem - this->m_uiDrawingIndex) * (iDefaultFontSize[1] + LB_ITEM_GAP * 2)) && (this->m_vecMousePos[1] < this->m_vecPosition[1] + 3 + (int)(uiItem - this->m_uiDrawingIndex) * (iDefaultFontSize[1] + LB_ITEM_GAP * 2) + iDefaultFontSize[1] + LB_ITEM_GAP * 2)) {
				return true;
			}

			return false;
		}
	public:
		CListBox() : m_uiSelectedItem(std::string::npos), m_uiDrawingIndex(0), m_pHandler(nullptr) {}
		~CListBox() {}

		void AddItem(const std::wstring& wszDisplay, const std::wstring& wszData)
		{
			//Add listbox item

			listbox_item_s sItem;
			sItem.wszDisplayText = wszDisplay;
			sItem.wszDataContent = wszData;

			this->m_vItems.push_back(sItem);
		}

		void InsertAt(const size_t uiPos, const std::wstring& wszDisplay, const std::wstring& wszData)
		{
			//Insert at position

			listbox_item_s sItem;
			sItem.wszDisplayText = wszDisplay;
			sItem.wszDataContent = wszData;

			this->m_vItems.insert(this->m_vItems.begin() + uiPos, sItem);
		}

		void EditItem(const size_t uiId, const std::wstring& wszDisplay, const std::wstring& wszData)
		{
			//Edit listbox item

			if (uiId < this->m_vItems.size()) {
				this->m_vItems[uiId].wszDisplayText = wszDisplay;
				this->m_vItems[uiId].wszDataContent = wszData;
			}
		}

		void RemoveItem(const size_t uiId)
		{
			//Remove an item

			if (uiId < this->m_vItems.size()) {
				this->m_vItems.erase(this->m_vItems.begin() + uiId);
			}
		}

		const size_t GetCount(void) const
		{
			//Return item count

			return this->m_vItems.size();
		}

		std::wstring GetItem(const size_t uiId)
		{
			//Get item data value by ID

			if (uiId < this->m_vItems.size()) {
				return this->m_vItems[uiId].wszDataContent;
			}

			return L"";
		}

		std::wstring GetItemDisplayText(const size_t uiId)
		{
			//Get item display text by ID

			if (uiId < this->m_vItems.size()) {
				return this->m_vItems[uiId].wszDisplayText;
			}

			return L"";
		}

		size_t GetSelectedItem(void)
		{
			//Get selected item ID

			return this->m_uiSelectedItem;
		}

		void Clear(void)
		{
			//Clear all items

			this->m_vItems.clear();
		}

		void UpdateDimension(void)
		{
			//Update dimension

			this->m_uiItemDisplayCountY = this->m_vecSize[1] / (iDefaultFontSize[1] + LB_ITEM_GAP * 2);
		}

		void Draw(void)
		{
			//Draw component

			pRenderer->DrawBox(this->m_vecPosition[0], this->m_vecPosition[1], this->m_vecSize[0], this->m_vecSize[1], 1, this->m_sFrameColor.r, this->m_sFrameColor.g, this->m_sFrameColor.b, this->m_sFrameColor.a);
			pRenderer->DrawFilledBox(this->m_vecPosition[0] + 1, this->m_vecPosition[1] + 1, this->m_vecSize[0] - 1, this->m_vecSize[1] - 1, this->m_sFillColor.r, this->m_sFillColor.g, this->m_sFillColor.b, this->m_sFillColor.a);

			for (size_t i = this->m_uiDrawingIndex; i < this->m_uiDrawingIndex + this->m_uiItemDisplayCountY; i++) {
				if (i >= this->m_vItems.size()) {
					continue;
				}

				if (this->IsInItemRange(i)) {
					pRenderer->DrawFilledBox(this->m_vecPosition[0] + 1, this->m_vecPosition[1] + 3 + (int)(i - this->m_uiDrawingIndex) * (iDefaultFontSize[1] + LB_ITEM_GAP * 2) - iDefaultFontSize[1] / 2, this->m_vecSize[0] - 1, iDefaultFontSize[1] + LB_ITEM_GAP * 2, this->m_sHoverColor.r, this->m_sHoverColor.g, this->m_sHoverColor.b, this->m_sHoverColor.a);
				}

				if (i == this->m_uiSelectedItem) {
					pRenderer->DrawFilledBox(this->m_vecPosition[0] + 1, this->m_vecPosition[1] + 3 + (int)(i - this->m_uiDrawingIndex) * (iDefaultFontSize[1] + LB_ITEM_GAP * 2) - iDefaultFontSize[1] / 2, this->m_vecSize[0] - 1, iDefaultFontSize[1] + LB_ITEM_GAP * 2, this->m_sSelectColor.r, this->m_sSelectColor.g, this->m_sSelectColor.b, this->m_sSelectColor.a);
				}

				pRenderer->DrawString(pDefaultFont, this->m_vItems[i].wszDisplayText, this->m_vecPosition[0] + 5, this->m_vecPosition[1] + 3 + (int)(i - this->m_uiDrawingIndex) * (iDefaultFontSize[1] + LB_ITEM_GAP * 2), this->m_sTextColor.r, this->m_sTextColor.g, this->m_sTextColor.b, this->m_sTextColor.a);
			}
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);
			} else {
				if ((iMouseKey == 1) && (!bDown)) {
					for (size_t i = 0; i < this->m_vItems.size(); i++) {
						if (this->IsInItemRange(i)) {
							this->m_uiSelectedItem = i;

							if (this->m_pHandler) {
								this->m_pHandler->OnListBoxSelectEvent(this, i);
							}

							break;
						}
					}
				}
			}
		}

		void OnMouseWheelEvent(short wDistance, bool bForward)
		{
			//Handle mouse wheel event

			if (!bForward) {
				this->ScrollUp();
			} else {
				this->ScrollDown();
			}
		}

		void ScrollUp(void)
		{
			//Scroll list up

			int iTopValue = (int)this->m_vItems.size() - (int)this->m_uiItemDisplayCountY;
			if (iTopValue < 0) {
				iTopValue = 0;
			}

			if (this->m_uiDrawingIndex < (size_t)iTopValue) {
				this->m_uiDrawingIndex++;
			}
		}

		void ScrollDown(void)
		{
			//Scroll list down

			if (this->m_uiDrawingIndex > 0) {
				this->m_uiDrawingIndex--;
			}
		}

		//Setters
		void SetPosition(const Entity::Vector& vec) { this->m_vecPosition = vec; }
		void SetSize(const Entity::Vector& vec) { this->m_vecSize = vec; this->UpdateDimension(); }
		void SetOwner(IListBoxSelectHandler* pOwner) { this->m_pHandler = pOwner; }
		void SetFrameColor(const Entity::Color& col) { this->m_sFrameColor = col; }
		void SetFillColor(const Entity::Color& col) { this->m_sFillColor = col; }
		void SetTextColor(const Entity::Color& col) { this->m_sTextColor = col; }
		void SetHoverColor(const Entity::Color& col) { this->m_sHoverColor = col; }
		void SetSelectColor(const Entity::Color& col) { this->m_sSelectColor = col; }
	};

	/* Play menu component */
	class CPlayMenu : public IMenu, IButtonClickHandler, IListBoxSelectHandler {
	private:
		CButton m_oPlay;
		CListBox m_oSaveGames;
		CButton m_oLoad;
		DxSound::HDXSOUND m_hSelect;
		DxRenderer::HD3DSPRITE m_hUp;
		DxRenderer::HD3DSPRITE m_hDown;
		Entity::Vector m_vecMousePos;

		void AcquireSavedGameStates(void)
		{
			//Acquire all saved game states

			WIN32_FIND_DATA sFindData = { 0 };

			HANDLE hFileSearch = FindFirstFile((wszBasePath + L"saves\\*.*").c_str(), &sFindData);
			if (hFileSearch != INVALID_HANDLE_VALUE) {
				while (FindNextFile(hFileSearch, &sFindData)) {
					if (sFindData.cFileName[0] == '.') {
						continue;
					}

					if ((sFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) {
						if (Utils::ExtractFileExt(sFindData.cFileName) != L"sav") {
							continue;
						}

						std::wstring wszDisplayText;
						std::wstring wszDataContent;
						SYSTEMTIME stUTC, stLocal;
						wchar_t wszBuffer[MAX_PATH] = { 0 };

						FileTimeToSystemTime(&sFindData.ftLastWriteTime, &stUTC);
						SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

						StringCchPrintf(wszBuffer, sizeof(wszBuffer), TEXT("%02d/%02d/%d %02d:%02d:%02d"), stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);

						Entity::CSaveGameReader reader;
						reader.OpenSaveGameFile(Utils::ConvertToAnsiString(std::wstring(sFindData.cFileName)));
						reader.AcquireSaveGameData();
						wszDisplayText = Utils::ConvertToWideString(reader.GetDataItem("package") + " | " + reader.GetDataItem("map") + " | ") + std::wstring(wszBuffer);
						wszDataContent = std::wstring(sFindData.cFileName);
						reader.Close();

						this->m_oSaveGames.AddItem(wszDisplayText, wszDataContent);
					}
				}

				FindClose(hFileSearch);
			}
		}
	public:
		CPlayMenu() {}
		~CPlayMenu() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize component

			this->m_hUp = pRenderer->LoadSprite(wszBasePath + L"media\\gfx\\up.png", 1, 50, 50, 1, false);
			this->m_hDown = pRenderer->LoadSprite(wszBasePath + L"media\\gfx\\down.png", 1, 50, 50, 1, false);

			this->m_oPlay.SetText(L"Start new game");
			this->m_oPlay.SetPosition(Entity::Vector(250, 200));
			this->m_oPlay.SetSize(Entity::Vector(230, 35));
			this->m_oPlay.SetOwner(this);
			this->m_oPlay.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oPlay.SetFrameColor(Entity::Color(0, 0, 0, 150));
			this->m_oPlay.SetFillColor(Entity::Color(103, 225, 123, 150));
			this->m_oPlay.SetHoverColor(Entity::Color(143, 235, 155, 150));

			this->m_oSaveGames.SetOwner(this);
			this->m_oSaveGames.SetPosition(Entity::Vector(265, 300));
			this->m_oSaveGames.SetSize(Entity::Vector(600, 300));
			this->m_oSaveGames.SetFrameColor(Entity::Color(150, 150, 150, 150));
			this->m_oSaveGames.SetFillColor(Entity::Color(50, 50, 50, 150));
			this->m_oSaveGames.SetTextColor(Entity::Color(200, 200, 200, 255));
			this->m_oSaveGames.SetHoverColor(Entity::Color(100, 100, 100, 150));
			this->m_oSaveGames.SetSelectColor(Entity::Color(150, 150, 150, 150));

			this->m_oLoad.SetText(L"Load game");
			this->m_oLoad.SetPosition(Entity::Vector(265, 300 + 300 + 10));
			this->m_oLoad.SetSize(Entity::Vector(150, 35));
			this->m_oLoad.SetOwner(this);
			this->m_oLoad.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oLoad.SetFrameColor(Entity::Color(200, 200, 200, 150));
			this->m_oLoad.SetFillColor(Entity::Color(50, 50, 50, 150));
			this->m_oLoad.SetHoverColor(Entity::Color(100, 100, 100, 150));

			this->m_hSelect = pSound->QuerySound(wszBasePath + L"media\\sound\\listbox_select.wav");

			this->AcquireSavedGameStates();

			return true;
		}

		void AddSaveGameToList(const std::wstring& wszFile, FILETIME* fileTime = nullptr)
		{
			//Add a save game file to list

			std::wstring wszDisplayText;
			std::wstring wszDataContent;
			SYSTEMTIME stUTC, stLocal;
			std::wstring wszDateAndTime;

			if (fileTime != nullptr) {
				wchar_t wszBuffer[MAX_PATH] = { 0 };
				FileTimeToSystemTime(fileTime, &stUTC);
				SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

				StringCchPrintf(wszBuffer, sizeof(wszBuffer), TEXT("%02d/%02d/%d %02d:%02d:%02d"), stLocal.wMonth, stLocal.wDay, stLocal.wYear, stLocal.wHour, stLocal.wMinute, stLocal.wSecond);
			
				wszDateAndTime = wszBuffer;
			} else {
				tm time;
				time_t t = std::time(nullptr);
				localtime_s(&time, &t);

				std::ostringstream oss;
				oss << std::put_time(&time, "%m/%d/%Y %H:%M:%S");

				wszDateAndTime = Utils::ConvertToWideString(oss.str());
			}

			Entity::CSaveGameReader reader;
			reader.OpenSaveGameFile(Utils::ConvertToAnsiString(wszFile));
			reader.AcquireSaveGameData();
			wszDisplayText = Utils::ConvertToWideString(reader.GetDataItem("package") + " | " + reader.GetDataItem("map") + " | ") + std::wstring(wszDateAndTime);
			wszDataContent = std::wstring(wszFile);
			reader.Close();

			this->m_oSaveGames.InsertAt(0, wszDisplayText, wszDataContent);
		}

		virtual void Draw(void)
		{
			//Draw component

			this->m_oPlay.Draw();
			this->m_oSaveGames.Draw();
			this->m_oLoad.Draw();

			pRenderer->DrawSprite(this->m_hUp, 265 + 604, 300 + 300 / 2 - 50 / 2 - 10, 0, 0.0f);
			pRenderer->DrawSprite(this->m_hDown, 265 + 604, 300 + 300 / 2 - 50 / 2 + 50 - 10, 0, 0.0f);

			pRenderer->DrawString(pDefaultFont, L"Saved games", 265, 279, 200, 200, 200, 150);

			if (this->m_oSaveGames.GetCount() == 0) {
				const std::wstring wszHint = L"No saved games yet.";
				pRenderer->DrawString(pDefaultFont, wszHint, 265 + 300 - ((int)wszHint.length() * iDefaultFontSize[0] / 2), 300 + 150 - iDefaultFontSize[1] / 2, 200, 200, 200, 150);
			}
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);
			}

			this->m_oPlay.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSaveGames.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oLoad.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

			if ((iMouseKey == 1) && (!bDown)) {
				if ((this->m_vecMousePos[0] > 265 + 604) && (this->m_vecMousePos[0] < 265 + 604 + 50) && (this->m_vecMousePos[1] > 300 + 300 / 2 - 50 / 2 - 10) && (this->m_vecMousePos[1] < 300 + 300 / 2 - 50 / 2 - 10 + 50)) {
					this->m_oSaveGames.ScrollDown();
				}

				if ((this->m_vecMousePos[0] > 265 + 604) && (this->m_vecMousePos[0] < 265 + 604 + 50) && (this->m_vecMousePos[1] > 300 + 300 / 2 - 50 / 2 + 50 - 10) && (this->m_vecMousePos[1] < 300 + 300 / 2 - 50 / 2 + 50 - 10 + 50)) {
					this->m_oSaveGames.ScrollUp();
				}
			}
		}

		virtual void OnMouseWheelEvent(short wDistance, bool bForward)
		{
			//Handle mouse wheel event

			this->m_oSaveGames.OnMouseWheelEvent(wDistance, bForward);
		}

		virtual void OnButtonClick(class CButton* pButton);
		virtual void OnListBoxSelectEvent(class CListBox* pListBox, size_t uiItem);

		virtual void Release(void)
		{
		}
	};

	/* Package menu component */
	class CPackageMenu : public IMenu, public IButtonClickHandler, IImageListViewSelector {
	private:
		struct package_s {
			std::wstring wszIdent;
			std::wstring wszPath;
			DxRenderer::HD3DSPRITE hPreview;
			std::vector<std::wstring> vAboutContent;
		};

		std::vector<package_s> m_vPackages;
		size_t m_uiSelectedPackage;
		CButton m_oButton;
		CImageListView m_oImageListView;
		CButton m_oBrowse;
		DxRenderer::HD3DSPRITE m_hBackward;
		DxRenderer::HD3DSPRITE m_hForward;
		Entity::Vector m_vecMousePos;
	public:
		CPackageMenu() : m_uiSelectedPackage(std::string::npos) {}
		~CPackageMenu() {}

		virtual void AddPackage(const std::wstring& wszPackageName, const std::wstring& wszPackagePath)
		{
			//Load specific package

			package_s sPackage;

			sPackage.wszIdent = wszPackageName;
			sPackage.wszPath = wszPackagePath;
			sPackage.vAboutContent = Utils::ReadFileLines(wszPackagePath + L"\\about.txt");

			if (Utils::FileExists(wszPackagePath + L"\\preview.png")) {
				sPackage.hPreview = pRenderer->LoadSprite(wszPackagePath + L"\\preview.png", 1, 195, 90, 1, true);
			}
			else {
				sPackage.hPreview = pRenderer->LoadSprite(wszPackagePath + L"\\preview.jpg", 1, 195, 90, 1, true);
			}

			this->m_vPackages.push_back(sPackage);

			this->m_oImageListView.AddItem(sPackage.hPreview, sPackage.wszIdent);
		}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize packages

			//Load navigation sprites
			this->m_hBackward = pRenderer->LoadSprite(wszBasePath + L"media\\gfx\\backward.png", 1, 50, 50, 1, false);
			this->m_hForward = pRenderer->LoadSprite(wszBasePath + L"media\\gfx\\forward.png", 1, 50, 50, 1, false);

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

						if ((std::wstring(sFindData.cFileName) != L".common") && (std::wstring(sFindData.cFileName) != L"game")) { //Filter common include folder and actual main game folder from mods list
							sPackage.wszIdent = sFindData.cFileName;
							sPackage.wszPath = L"";
							sPackage.vAboutContent = Utils::ReadFileLines(wszBasePath + L"packages\\" + sPackage.wszIdent + L"\\about.txt");
							sPackage.hPreview = pRenderer->LoadSprite(wszBasePath + L"packages\\" + sPackage.wszIdent + L"\\preview.png", 1, 195, 90, 1, true);

							this->m_vPackages.push_back(sPackage);

							this->m_oImageListView.AddItem(sPackage.hPreview, sPackage.wszIdent);
						}
					}
				}

				FindClose(hFileSearch);
			}

			int iAboutContentHeight = 0;
			
			//Select first entry
			if (this->m_vPackages.size() > 0) {
				this->m_uiSelectedPackage = 0;

				iAboutContentHeight = (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1];
			}

			this->m_oButton.SetText(L"Play!");
			this->m_oButton.SetPosition(Entity::Vector(250, 200 + iAboutContentHeight));
			this->m_oButton.SetSize(Entity::Vector(130, 35));
			this->m_oButton.SetOwner(this);
			this->m_oButton.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oButton.SetFrameColor(Entity::Color(0, 0, 0, 150));
			this->m_oButton.SetFillColor(Entity::Color(103, 225, 123, 150));
			this->m_oButton.SetHoverColor(Entity::Color(143, 235, 155, 150));

			this->m_oImageListView.SetOwner(this);
			this->m_oImageListView.SetPosition(Entity::Vector(250, 200 + 45 + iAboutContentHeight));
			this->m_oImageListView.SetSize(Entity::Vector(700, 400));
			this->m_oImageListView.SetImageGap(10);
			this->m_oImageListView.SetImageSize(Entity::Vector(195, 90));
			this->m_oImageListView.SetHoverColor(Entity::Color(0, 150, 0, 150));
			this->m_oImageListView.SetSelectColor(Entity::Color(0, 200, 0, 150));
			this->m_oImageListView.UpdateDimensions();

			this->m_oBrowse.SetText(L"Browse Workshop");
			this->m_oBrowse.SetPosition(Entity::Vector(450, 200 + iAboutContentHeight));
			this->m_oBrowse.SetSize(Entity::Vector(150, 35));
			this->m_oBrowse.SetOwner(this);
			this->m_oBrowse.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oBrowse.SetFrameColor(Entity::Color(255, 255, 255, 150));
			this->m_oBrowse.SetFillColor(Entity::Color(50, 50, 50, 150));
			this->m_oBrowse.SetHoverColor(Entity::Color(150, 150, 150, 150));
			
			return true;
		}

		virtual void Draw(void)
		{
			//Draw menu

			if (!this->m_bActive)
				return;

			const int iStartPosX = 250;
			const int iStartPosY = 200;

			//Draw hint if there are no packages found
			if (this->m_vPackages.size() == 0) {
				pRenderer->DrawString(pDefaultFont, L"No mods found", iStartPosX, iStartPosY, 200, 200, 200, 255);
			}

			//Draw selected package preview
			if ((this->m_uiSelectedPackage != std::string::npos) && (this->m_uiSelectedPackage < this->m_vPackages.size())) {
				//Draw about content
				for (size_t i = 0; i < this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size(); i++) {
					pRenderer->DrawString(pDefaultFont, this->m_vPackages[this->m_uiSelectedPackage].vAboutContent[i], iStartPosX, iStartPosY + (int)i * iDefaultFontSize[1], 200, 200, 200, 150);
				}

				//Draw play button
				this->m_oButton.Draw();
			}

			//Draw listview
			this->m_oImageListView.Draw();

			//Draw Workshop browse button if required
			if (pAppSteamID->iValue != 0) {
				this->m_oBrowse.Draw();
			}

			//Draw navigation
			if (this->m_vPackages.size() > 0) {
				int iAboutContentHeight = 0;
				if (this->m_vPackages.size() > 0) {
					iAboutContentHeight = (int)this->m_vPackages[this->m_uiSelectedPackage].vAboutContent.size() * iDefaultFontSize[1];
				}

				pRenderer->DrawSprite(this->m_hBackward, 250, 200 + 45 + iAboutContentHeight + 350, 0, 0.0f);
				pRenderer->DrawSprite(this->m_hForward, 300, 200 + 45 + iAboutContentHeight + 350, 0, 0.0f);
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

	/* Tab menu component */
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
		DxSound::HDXSOUND m_hSelect;
	public:
		CTabMenu() : m_uiSelectedTab(std::string::npos), m_uiHoverItem(std::string::npos), m_iTabSpace(20) {}
		~CTabMenu() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize component

			this->m_hSelect = pSound->QuerySound(wszBasePath + L"media\\sound\\mainmenu_select.wav");

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

					pSound->Play(this->m_hSelect, pSndVolume->iValue, 0);
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
		virtual void SetActiveTab(size_t uiIndex) { this->m_uiSelectedTab = uiIndex; }
	};

	/* Key binding component */
	const int BK_OBJECT_WIDTH = 290;
	const int BK_OBJECT_HEIGHT = 35;
	class CKeyBinding : public IMenu {
	private:
		std::wstring m_wszCommand;
		int m_vKey;
		std::wstring m_wszVKeyName;
		bool m_bInSet;
		bool m_bHover;
		Entity::Vector m_vecPos;
		Entity::Vector m_vecMousePos;

		void UpdateKeyValue(int vKey)
		{
			//Update current key value

			wchar_t wCharacter = MapVirtualKey(vKey, MAPVK_VK_TO_CHAR);

			this->m_wszVKeyName = wCharacter;
			this->m_vKey = vKey;
		}

		bool IsInsideComponent(void)
		{
			//Check if cursor is inside component range

			if ((this->m_vecMousePos[0] > this->m_vecPos[0]) && (this->m_vecMousePos[0] < this->m_vecPos[0] + BK_OBJECT_WIDTH) && (this->m_vecMousePos[1] > this->m_vecPos[1]) && (this->m_vecMousePos[1] < this->m_vecPos[1] + BK_OBJECT_HEIGHT)) {
				return true;
			}

			return false;
		}
	public:
		CKeyBinding() : m_bInSet(false), m_bHover(false) {}
		~CKeyBinding() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize object

			return true;
		}

		virtual void Draw(void)
		{
			//Draw menu
			
			pRenderer->DrawBox(this->m_vecPos[0], this->m_vecPos[1], BK_OBJECT_WIDTH, BK_OBJECT_HEIGHT, 1, 50, 50, 50, 150);

			if (this->m_bHover) {
				pRenderer->DrawFilledBox(this->m_vecPos[0] + 1, this->m_vecPos[1] + 1, BK_OBJECT_WIDTH / 2 - 1, BK_OBJECT_HEIGHT - 1, 150, 150, 150, 150);
			}

			if (!this->m_bInSet) {
				pRenderer->DrawFilledBox(this->m_vecPos[0] + BK_OBJECT_WIDTH / 2, this->m_vecPos[1] + 1, BK_OBJECT_WIDTH / 2, BK_OBJECT_HEIGHT - 1, 34, 175, 76, 150);
			} else {
				pRenderer->DrawFilledBox(this->m_vecPos[0] + BK_OBJECT_WIDTH / 2, this->m_vecPos[1] + 1, BK_OBJECT_WIDTH / 2, BK_OBJECT_HEIGHT - 1, 130, 232, 160, 150);
			}

			Entity::Color sCmdColor;

			if (this->m_bHover) {
				sCmdColor = Entity::Color(50, 50, 50, 150);
			} else {
				sCmdColor = Entity::Color(250, 250, 250, 150);
			}

			pRenderer->DrawString(pDefaultFont, this->m_wszCommand, this->m_vecPos[0] + 5, this->m_vecPos[1] + BK_OBJECT_HEIGHT / 2 - iDefaultFontSize[1] / 2, sCmdColor.r, sCmdColor.g, sCmdColor.b, sCmdColor.a);
			pRenderer->DrawString(pDefaultFont, this->m_wszVKeyName, this->m_vecPos[0] + BK_OBJECT_WIDTH - BK_OBJECT_WIDTH / 4 - ((int)this->m_wszVKeyName.length() / 2 * iDefaultFontSize[0]), this->m_vecPos[1] + BK_OBJECT_HEIGHT / 2 - iDefaultFontSize[1] / 2, 250, 250, 250, 150);
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			if (!iMouseKey) {
				this->m_vecMousePos = Entity::Vector(x, y);

				this->m_bHover = this->IsInsideComponent();
			} else {
				if ((iMouseKey == 1) && (!bDown)) {
					if (this->IsInsideComponent()) {
						this->m_bInSet = !this->m_bInSet;
					}
				}
			}
		}

		virtual void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle key events
			
			if ((this->m_bInSet) && (!bDown)) {
				this->UpdateKeyValue(vKey);

				this->m_bInSet = false;
			}
		}

		virtual void Release(void)
		{
		}

		void SetCommand(const std::wstring& wszCommand)
		{
			//Set command

			this->m_wszCommand = wszCommand;
		}

		void SetVkey(int vKey)
		{
			//Set vkey

			this->UpdateKeyValue(vKey);
		}

		void SetPosition(const Entity::Vector& vecPos)
		{
			//Set position

			this->m_vecPos = vecPos;
		}

		//Getters
		const std::wstring& GetCommand(void) const { return this->m_wszCommand; }
		int GetVkey(void) const { return this->m_vKey; }
	};

	/* Keys submenu component */
	class CSettingsKeys : public IMenu, public IButtonClickHandler {
	private:
		Entity::Vector m_vecPos;
		CKeyBinding m_oMoveForward;
		CKeyBinding m_oMoveBackward;
		CKeyBinding m_oTurnLeft;
		CKeyBinding m_oTurnRight;
		CKeyBinding m_oStrafeLeft;
		CKeyBinding m_oStrafeRight;
		CKeyBinding m_oAttack;
		CKeyBinding m_oUse;
		CKeyBinding m_oThrow;
		CKeyBinding m_oSlot1;
		CKeyBinding m_oSlot2;
		CKeyBinding m_oSlot3;
		CKeyBinding m_oSlot4;
		CKeyBinding m_oSlot5;
		CKeyBinding m_oMenu;
		CKeyBinding m_oQuickSave;
		CKeyBinding m_oConsole;
		CButton m_btnSave;
		CButton m_btnRestoreDefaults;

		void SaveBindings(void)
		{
			//Save key bindings

			g_oInputMgr.SetKeyBindingCode(L"MOVE_FORWARD", this->m_oMoveForward.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"MOVE_BACKWARD", this->m_oMoveBackward.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"TURN_LEFT", this->m_oTurnLeft.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"TURN_RIGHT", this->m_oTurnRight.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"MOVE_LEFT", this->m_oStrafeLeft.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"MOVE_RIGHT", this->m_oStrafeRight.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"ATTACK", this->m_oAttack.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"USE", this->m_oUse.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"THROW", this->m_oThrow.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"SLOT1", this->m_oSlot1.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"SLOT2", this->m_oSlot2.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"SLOT3", this->m_oSlot3.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"SLOT4", this->m_oSlot4.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"SLOT5", this->m_oSlot5.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"MENU", this->m_oMenu.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"SAVEGAME", this->m_oQuickSave.GetVkey());
			g_oInputMgr.SetKeyBindingCode(L"CONSOLE", this->m_oConsole.GetVkey());

			std::wofstream hFile;
			hFile.open(wszBasePath + L"bindings.cfg", std::wifstream::out);
			if (hFile.is_open()) {
				hFile << L"# Key bindings configuration file" << std::endl << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oMoveForward.GetVkey()) + L" \"" + this->m_oMoveForward.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oMoveBackward.GetVkey()) + L" \"" + this->m_oMoveBackward.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oTurnLeft.GetVkey()) + L" \"" + this->m_oTurnLeft.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oTurnRight.GetVkey()) + L" \"" + this->m_oTurnRight.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oStrafeLeft.GetVkey()) + L" \"" + this->m_oStrafeLeft.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oStrafeRight.GetVkey()) + L" \"" + this->m_oStrafeRight.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oAttack.GetVkey()) + L" \"" + this->m_oAttack.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oUse.GetVkey()) + L" \"" + this->m_oUse.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oThrow.GetVkey()) + L" \"" + this->m_oThrow.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oSlot1.GetVkey()) + L" \"" + this->m_oSlot1.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oSlot2.GetVkey()) + L" \"" + this->m_oSlot2.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oSlot3.GetVkey()) + L" \"" + this->m_oSlot3.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oSlot4.GetVkey()) + L" \"" + this->m_oSlot4.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oSlot5.GetVkey()) + L" \"" + this->m_oSlot5.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oMenu.GetVkey()) + L" \"" + this->m_oMenu.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oQuickSave.GetVkey()) + L" \"" + this->m_oQuickSave.GetCommand() + L"\"" << std::endl;
				hFile << L"bind " + std::to_wstring(this->m_oConsole.GetVkey()) + L" \"" + this->m_oConsole.GetCommand() + L"\"" << std::endl;

				hFile.close();
			}
		}

		void RestoreDefaults(void)
		{
			//Restore default key bindings

			this->m_oMoveForward.SetVkey(87);
			this->m_oMoveBackward.SetVkey(83);
			this->m_oTurnLeft.SetVkey(39);
			this->m_oTurnRight.SetVkey(37);
			this->m_oStrafeLeft.SetVkey(65);
			this->m_oStrafeRight.SetVkey(68);
			this->m_oAttack.SetVkey(32);
			this->m_oUse.SetVkey(69);
			this->m_oThrow.SetVkey(71);
			this->m_oSlot1.SetVkey(49);
			this->m_oSlot2.SetVkey(50);
			this->m_oSlot3.SetVkey(51);
			this->m_oSlot4.SetVkey(52);
			this->m_oSlot5.SetVkey(53);
			this->m_oMenu.SetVkey(27);
			this->m_oQuickSave.SetVkey(113);
			this->m_oConsole.SetVkey(112);
		}
	public:
		CSettingsKeys() {}
		~CSettingsKeys() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize component

			this->m_oMoveForward.Initialize(0, 0, nullptr);
			this->m_oMoveForward.SetCommand(L"MOVE_FORWARD");
			this->m_oMoveForward.SetVkey(g_oInputMgr.GetKeyBindingCode(L"MOVE_FORWARD"));
			this->m_oMoveForward.SetPosition(Entity::Vector(250, 250));

			this->m_oMoveBackward.Initialize(0, 0, nullptr);
			this->m_oMoveBackward.SetCommand(L"MOVE_BACKWARD");
			this->m_oMoveBackward.SetVkey(g_oInputMgr.GetKeyBindingCode(L"MOVE_BACKWARD"));
			this->m_oMoveBackward.SetPosition(Entity::Vector(250, 300));

			this->m_oTurnLeft.Initialize(0, 0, nullptr);
			this->m_oTurnLeft.SetCommand(L"TURN_LEFT");
			this->m_oTurnLeft.SetVkey(g_oInputMgr.GetKeyBindingCode(L"TURN_LEFT"));
			this->m_oTurnLeft.SetPosition(Entity::Vector(250, 350));

			this->m_oTurnRight.Initialize(0, 0, nullptr);
			this->m_oTurnRight.SetCommand(L"TURN_RIGHT");
			this->m_oTurnRight.SetVkey(g_oInputMgr.GetKeyBindingCode(L"TURN_RIGHT"));
			this->m_oTurnRight.SetPosition(Entity::Vector(250, 400));

			this->m_oStrafeLeft.Initialize(0, 0, nullptr);
			this->m_oStrafeLeft.SetCommand(L"MOVE_LEFT");
			this->m_oStrafeLeft.SetVkey(g_oInputMgr.GetKeyBindingCode(L"MOVE_LEFT"));
			this->m_oStrafeLeft.SetPosition(Entity::Vector(250, 450));

			this->m_oStrafeRight.Initialize(0, 0, nullptr);
			this->m_oStrafeRight.SetCommand(L"MOVE_RIGHT");
			this->m_oStrafeRight.SetVkey(g_oInputMgr.GetKeyBindingCode(L"MOVE_RIGHT"));
			this->m_oStrafeRight.SetPosition(Entity::Vector(250, 500));

			this->m_oAttack.Initialize(0, 0, nullptr);
			this->m_oAttack.SetCommand(L"ATTACK");
			this->m_oAttack.SetVkey(g_oInputMgr.GetKeyBindingCode(L"ATTACK"));
			this->m_oAttack.SetPosition(Entity::Vector(550, 250));

			this->m_oUse.Initialize(0, 0, nullptr);
			this->m_oUse.SetCommand(L"USE");
			this->m_oUse.SetVkey(g_oInputMgr.GetKeyBindingCode(L"USE"));
			this->m_oUse.SetPosition(Entity::Vector(550, 300));

			this->m_oThrow.Initialize(0, 0, nullptr);
			this->m_oThrow.SetCommand(L"THROW");
			this->m_oThrow.SetVkey(g_oInputMgr.GetKeyBindingCode(L"THROW"));
			this->m_oThrow.SetPosition(Entity::Vector(550, 350));

			this->m_oSlot1.Initialize(0, 0, nullptr);
			this->m_oSlot1.SetCommand(L"SLOT1");
			this->m_oSlot1.SetVkey(g_oInputMgr.GetKeyBindingCode(L"SLOT1"));
			this->m_oSlot1.SetPosition(Entity::Vector(250, 550));

			this->m_oSlot2.Initialize(0, 0, nullptr);
			this->m_oSlot2.SetCommand(L"SLOT2");
			this->m_oSlot2.SetVkey(g_oInputMgr.GetKeyBindingCode(L"SLOT2"));
			this->m_oSlot2.SetPosition(Entity::Vector(550, 550));

			this->m_oSlot3.Initialize(0, 0, nullptr);
			this->m_oSlot3.SetCommand(L"SLOT3");
			this->m_oSlot3.SetVkey(g_oInputMgr.GetKeyBindingCode(L"SLOT3"));
			this->m_oSlot3.SetPosition(Entity::Vector(250, 600));

			this->m_oSlot4.Initialize(0, 0, nullptr);
			this->m_oSlot4.SetCommand(L"SLOT4");
			this->m_oSlot4.SetVkey(g_oInputMgr.GetKeyBindingCode(L"SLOT4"));
			this->m_oSlot4.SetPosition(Entity::Vector(550, 600));

			this->m_oSlot5.Initialize(0, 0, nullptr);
			this->m_oSlot5.SetCommand(L"SLOT5");
			this->m_oSlot5.SetVkey(g_oInputMgr.GetKeyBindingCode(L"SLOT5"));
			this->m_oSlot5.SetPosition(Entity::Vector(250, 650));

			this->m_oMenu.Initialize(0, 0, nullptr);
			this->m_oMenu.SetCommand(L"MENU");
			this->m_oMenu.SetVkey(g_oInputMgr.GetKeyBindingCode(L"MENU"));
			this->m_oMenu.SetPosition(Entity::Vector(550, 400));

			this->m_oQuickSave.Initialize(0, 0, nullptr);
			this->m_oQuickSave.SetCommand(L"SAVEGAME");
			this->m_oQuickSave.SetVkey(g_oInputMgr.GetKeyBindingCode(L"SAVEGAME"));
			this->m_oQuickSave.SetPosition(Entity::Vector(550, 450));

			this->m_oConsole.Initialize(0, 0, nullptr);
			this->m_oConsole.SetCommand(L"CONSOLE");
			this->m_oConsole.SetVkey(g_oInputMgr.GetKeyBindingCode(L"CONSOLE"));
			this->m_oConsole.SetPosition(Entity::Vector(550, 500));

			this->m_btnSave.SetOwner(this);
			this->m_btnSave.SetFillColor(Entity::Color(50, 135, 0, 150));
			this->m_btnSave.SetFrameColor(Entity::Color(250, 250, 250, 150));
			this->m_btnSave.SetHoverColor(Entity::Color(76, 155, 0, 150));
			this->m_btnSave.SetTextColor(Entity::Color(250, 250, 250, 150));
			this->m_btnSave.SetPosition(Entity::Vector(250, 730));
			this->m_btnSave.SetSize(Entity::Vector(200, 35));
			this->m_btnSave.SetText(L"Save");

			this->m_btnRestoreDefaults.SetOwner(this);
			this->m_btnRestoreDefaults.SetFillColor(Entity::Color(200, 200, 200, 150));
			this->m_btnRestoreDefaults.SetFrameColor(Entity::Color(0, 0, 0, 150));
			this->m_btnRestoreDefaults.SetHoverColor(Entity::Color(250, 250, 250, 150));
			this->m_btnRestoreDefaults.SetTextColor(Entity::Color(0, 0, 0, 150));
			this->m_btnRestoreDefaults.SetPosition(Entity::Vector(500, 730));
			this->m_btnRestoreDefaults.SetSize(Entity::Vector(200, 35));
			this->m_btnRestoreDefaults.SetText(L"Reset defaults");

			return true;
		}

		virtual void Draw(void)
		{
			//Draw component

			this->m_oMoveForward.Draw();
			this->m_oMoveBackward.Draw();
			this->m_oTurnLeft.Draw();
			this->m_oTurnRight.Draw();
			this->m_oStrafeLeft.Draw();
			this->m_oStrafeRight.Draw();
			this->m_oAttack.Draw();
			this->m_oUse.Draw();
			this->m_oThrow.Draw();
			this->m_oSlot1.Draw();
			this->m_oSlot2.Draw();
			this->m_oSlot3.Draw();
			this->m_oSlot4.Draw();
			this->m_oSlot5.Draw();
			this->m_oMenu.Draw();
			this->m_oQuickSave.Draw();
			this->m_oConsole.Draw();

			this->m_btnSave.Draw();
			this->m_btnRestoreDefaults.Draw();
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oMoveForward.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oMoveBackward.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oTurnLeft.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oTurnRight.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oStrafeLeft.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oStrafeRight.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oAttack.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oUse.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oThrow.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot1.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot2.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot3.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot4.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot5.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oQuickSave.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oConsole.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

			this->m_btnSave.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_btnRestoreDefaults.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle key events

			this->m_oMoveForward.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oMoveBackward.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oTurnLeft.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oTurnRight.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oStrafeLeft.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oStrafeRight.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oAttack.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oUse.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oThrow.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot1.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot2.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot3.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot4.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oSlot5.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oMenu.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oQuickSave.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oConsole.OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void Release(void)
		{
		}

		virtual void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }

		virtual void OnButtonClick(class CButton* pButton);
	};

	/* Graphics settings submenu component */
	class CSettingsGfx : public IMenu, IButtonClickHandler {
	private:
		Entity::Vector m_vecPos;
		CComboBox m_oResolutions;
		CCheckbox m_oFullscreen;
		CButton m_btnSave;
		std::vector<std::wstring> m_vResolutions;

		void ReadResolutions(void)
		{
			//Read resultion data from file

			std::wifstream hFile;
			hFile.open(wszBasePath + L"resolutions.txt");
			if (hFile.is_open()) {
				wchar_t wszInputBuffer[1024];

				while (!hFile.eof()) {
					hFile.getline(wszInputBuffer, sizeof(wszInputBuffer), '\n');

					this->m_vResolutions.push_back(wszInputBuffer);
				}

				hFile.close();
			}
		}

		void SaveGfxSettings(void)
		{
			//Save graphics settings

			std::wstring wszWidth = L"1024";
			std::wstring wszHeight = L"768";
			std::wstring wszSelectedResolution = this->m_oResolutions.GetSelectedItem();
			size_t uiDelim = wszSelectedResolution.find(L"x");
			if (uiDelim != std::string::npos) {
				wszWidth = wszSelectedResolution.substr(0, uiDelim);
				wszHeight = wszSelectedResolution.substr(uiDelim + 1);
			}

			std::wofstream hFile;
			hFile.open(wszBasePath + L"gfx.cfg", std::wofstream::out);
			if (hFile.is_open()) {
				hFile << L"# Sound settings configuration file" << std::endl << std::endl;
				hFile << L"gfx_resolution_width " + wszWidth << std::endl;
				hFile << L"gfx_resolution_height " + wszHeight << std::endl;
				hFile << L"gfx_fullscreen " << ((this->m_oFullscreen.IsChecked()) ? L"1" : L"0") << std::endl;

				hFile.close();
			}
		}
	public:
		CSettingsGfx() {}
		~CSettingsGfx() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize component

			this->ReadResolutions();

			this->m_oResolutions.SetFillColor(Entity::Color(50, 50, 50, 150));
			this->m_oResolutions.SetFrameColor(Entity::Color(255, 255, 255, 150));
			this->m_oResolutions.SetHoverColor(Entity::Color(0, 50, 125, 150));
			this->m_oResolutions.SetPosition(Entity::Vector(250, 250));
			this->m_oResolutions.SetSelectedItem(0);
			this->m_oResolutions.SetTextColor(Entity::Color(200, 200, 200, 255));
			this->m_oResolutions.SetWidth(200);

			for (size_t i = 0; i < this->m_vResolutions.size(); i++) {
				this->m_oResolutions.AddItem(this->m_vResolutions[i]);

				if (std::to_wstring(pWindow->GetResolutionX()) + L"x" + std::to_wstring(pWindow->GetResolutionY()) == this->m_vResolutions[i]) {
					this->m_oResolutions.SetSelectedItem(i);
				}
			}

			this->m_oFullscreen.SetCheckColor(Entity::Color(50, 135, 0, 150));
			this->m_oFullscreen.SetChecked(pGfxFullscreen->bValue);
			this->m_oFullscreen.SetFrameColor(Entity::Color(200, 200, 200, 150));
			this->m_oFullscreen.SetHoverColor(Entity::Color(255, 255, 255, 150));
			this->m_oFullscreen.SetLabel(L"Run in fullscreen");
			this->m_oFullscreen.SetLabelColor(Entity::Color(200, 200, 200, 150));
			this->m_oFullscreen.SetPosition(Entity::Vector(250, 300));

			this->m_btnSave.SetOwner(this);
			this->m_btnSave.SetFillColor(Entity::Color(50, 135, 0, 150));
			this->m_btnSave.SetFrameColor(Entity::Color(250, 250, 250, 150));
			this->m_btnSave.SetHoverColor(Entity::Color(76, 155, 0, 150));
			this->m_btnSave.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_btnSave.SetPosition(Entity::Vector(250, 600));
			this->m_btnSave.SetSize(Entity::Vector(200, 35));
			this->m_btnSave.SetText(L"Save");

			return true;
		}

		virtual void Draw(void)
		{
			//Draw component

			this->m_oFullscreen.Draw();
			this->m_oResolutions.Draw();
			this->m_btnSave.Draw();

			if (!this->m_oResolutions.IsOpen()) {
				pRenderer->DrawString(pDefaultFont, L"Note: Saving graphic settings will trigger an application restart", 250, 340, 150, 150, 0, 255);
			}
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oResolutions.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

			if (!this->m_oResolutions.IsOpen()) {
				this->m_oFullscreen.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			}

			this->m_btnSave.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void Release(void)
		{
		}

		virtual void OnButtonClick(class CButton* pButton);

		virtual void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
	};

	/* Sound settings submenu component */
	class CSettingsSnd : public IMenu, IButtonClickHandler {
	private:
		Entity::Vector m_vecPos;
		CSlider m_oVolume;
		CButton m_btnSave;

		void SaveSoundSettings(void)
		{
			//Save sound settings

			pSndVolume->iValue = this->m_oVolume.GetValue();

			std::wofstream hFile;
			hFile.open(wszBasePath + L"sound.cfg", std::wofstream::out);
			if (hFile.is_open()) {
				hFile << L"# Sound settings configuration file" << std::endl << std::endl;
				hFile << L"snd_volume " + std::to_wstring(this->m_oVolume.GetValue()) << std::endl;

				hFile.close();
			}
		}
	public:
		CSettingsSnd() {}
		~CSettingsSnd() {}

		virtual bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize component

			this->m_oVolume.SetHintColor(Entity::Color(200, 200, 200, 255));
			this->m_oVolume.SetHoverColor(Entity::Color(200, 200, 200, 150));
			this->m_oVolume.SetLineColor(Entity::Color(150, 150, 150, 150));
			this->m_oVolume.SetMaxValue(10 + 1);
			this->m_oVolume.SetPosition(Entity::Vector(250, 300));
			this->m_oVolume.SetSliderColor(Entity::Color(150, 150, 0, 150));
			this->m_oVolume.SetValue(pSndVolume->iValue);
			this->m_oVolume.SetWidth(200);

			this->m_btnSave.SetOwner(this);
			this->m_btnSave.SetFillColor(Entity::Color(50, 135, 0, 150));
			this->m_btnSave.SetFrameColor(Entity::Color(250, 250, 250, 150));
			this->m_btnSave.SetHoverColor(Entity::Color(76, 155, 0, 150));
			this->m_btnSave.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_btnSave.SetPosition(Entity::Vector(250, 600));
			this->m_btnSave.SetSize(Entity::Vector(200, 35));
			this->m_btnSave.SetText(L"Save");

			return true;
		}

		virtual void Draw(void)
		{
			//Draw controls

			pRenderer->DrawString(pDefaultFont, L"Volume: ", this->m_vecPos[0], this->m_vecPos[1], 200, 200, 200, 255);

			this->m_oVolume.Draw();

			this->m_btnSave.Draw();
		}

		virtual void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oVolume.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_btnSave.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void Release(void)
		{
		}

		virtual void OnButtonClick(class CButton* pButton);

		virtual void SetPosition(const Entity::Vector& vec) { this->m_vecPos = vec; }
	};

	/* Settings menu component */
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

			this->m_oMenuKeys.Initialize(0, 0, nullptr);
			this->m_oMenuGfx.Initialize(0, 0, nullptr);
			this->m_oMenuSnd.Initialize(0, 0, nullptr);
			this->m_oMenuKeys.SetPosition(Entity::Vector(250, 250));
			this->m_oMenuGfx.SetPosition(Entity::Vector(250, 250));
			this->m_oMenuSnd.SetPosition(Entity::Vector(250, 250));

			this->m_oTabMenu.Initialize(w, h, pGameStarted);
			this->m_oTabMenu.AddItem(L"Bindings", &this->m_oMenuKeys);
			this->m_oTabMenu.AddItem(L"Graphics", &this->m_oMenuGfx);
			this->m_oTabMenu.AddItem(L"Sound   ", &this->m_oMenuSnd);

			this->m_oTabMenu.SetPosition(Entity::Vector(250, 200));
			this->m_oTabMenu.SetTabSpacing(20);
			this->m_oTabMenu.SetTextColor(Entity::Color(200, 200, 200, 255));
			this->m_oTabMenu.SetHoverColor(Entity::Color(50, 200, 0, 150));
			this->m_oTabMenu.SetSelectedColor(Entity::Color(10, 150, 0, 150));

			this->m_oTabMenu.SetActiveTab(0);

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

	/* GUI cursor component */
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

			this->m_hCursor = pRenderer->LoadSprite(wszBasePath + L"media\\gfx\\menucursor.png", 1, 16, 16, 1, false);
			if (this->m_hCursor == GFX_INVALID_SPRITE_ID) {
				return false;
			}

			return true;
		}

		void Release(void)
		{
			//Release resource

			//pRenderer->FreeSprite(this->m_hCursor);
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

	/* Main menu container component */
	class CMenu {
	private:
		CMainMenu m_oMainMenu;
		CPlayMenu m_oPlayMenu;
		CPackageMenu m_oPackageMenu;
		CSettingsMenu m_oSettingsMenu;
		bool m_bOpen;
		int m_iMouseX;
		int m_iMouseY;
		DxSound::HDXSOUND m_hSelect;
	public:
		CMenu() : m_bOpen(false) {}
		~CMenu() {}

		bool Initialize(int w, int h, bool* pGameStarted)
		{
			//Initialize menu

			if (!this->m_oMainMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			if (!this->m_oPlayMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			if (!this->m_oPackageMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			if (!this->m_oSettingsMenu.Initialize(w, h, pGameStarted)) {
				return false;
			}

			this->m_hSelect = pSound->QuerySound(wszBasePath + L"media\\sound\\mainmenu_select.wav");

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

			if (this->m_oPlayMenu.IsActive()) {
				this->m_oPlayMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
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
			
			if (this->m_oPlayMenu.IsActive()) {
				this->m_oPlayMenu.OnMouseWheelEvent(wDistance, bForward);
			}

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

				if (this->m_oPlayMenu.IsActive()) {
					this->m_oPlayMenu.Draw();
				} else if (this->m_oPackageMenu.IsActive()) {
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

		void AddPackage(const std::wstring& wszName, const std::wstring& wszPath)
		{
			//Pass to package menu

			this->m_oPackageMenu.AddPackage(wszName, wszPath);
		}

		void SetOpenStatus(bool status)
		{
			this->m_bOpen = status;

			if (status) {
				this->m_oMainMenu.OnToggleGameActiveMenuItems();
			}
		}

		void OpenPlayMenu(void)
		{
			this->m_oPlayMenu.SetActiveStatus(true);
			this->m_oPackageMenu.SetActiveStatus(false);
			this->m_oSettingsMenu.SetActiveStatus(false);

			pSound->Play(this->m_hSelect, pSndVolume->iValue, 0);
		}

		void OpenPackageMenu(void)
		{
			this->m_oPackageMenu.SetActiveStatus(true);
			this->m_oPlayMenu.SetActiveStatus(false);
			this->m_oSettingsMenu.SetActiveStatus(false);

			pSound->Play(this->m_hSelect, pSndVolume->iValue, 0);
		}

		void OpenSettingsMenu(void)
		{
			this->m_oSettingsMenu.SetActiveStatus(true);
			this->m_oPackageMenu.SetActiveStatus(false);
			this->m_oPlayMenu.SetActiveStatus(false);

			pSound->Play(this->m_hSelect, pSndVolume->iValue, 0);
		}

		void OnCloseAll(void)
		{
			this->m_oPlayMenu.SetActiveStatus(false);
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

		void AddToSaveGameList(const std::wstring& wszFile)
		{
			this->m_oPlayMenu.AddSaveGameToList(wszFile);
		}
	};

	/* Intermission menu component */
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

			pRenderer->DrawString(pDefaultFont, L"Your achieved score: " + std::to_wstring(this->m_iScore), this->m_vecPos[0] + 20, this->m_vecPos[1] + 100, 200, 200, 200, 255);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oButton.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void OnButtonClick(class CButton* pButton);
	};

	/* Game over menu component */
	class CGameOverMenu : public IButtonClickHandler {
	private:
		CForm m_oForm;
		CButton m_oBtnRestart;
		CButton m_oBtnReturnToMainMenu;
		CButton m_oBtnLoadLastSavedGame;
		std::wstring m_wszTitle;
		Entity::Vector m_vecPos;
		Entity::Vector m_vecSize;
	public:
		CGameOverMenu() {}
		~CGameOverMenu() {}

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
			this->m_oForm.SetTitleTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oForm.SetTitle(L"Game over!");

			this->m_oBtnRestart.SetFrameColor(Entity::Color(200, 200, 200, 150));
			this->m_oBtnRestart.SetFillColor(Entity::Color(50, 150, 50, 150));
			this->m_oBtnRestart.SetHoverColor(Entity::Color(70, 180, 70, 150));
			this->m_oBtnRestart.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oBtnRestart.SetSize(Entity::Vector(200, 35));
			this->m_oBtnRestart.SetOwner(this);
			this->m_oBtnRestart.SetText(L"Try again");

			this->m_oBtnLoadLastSavedGame.SetFrameColor(Entity::Color(200, 200, 200, 150));
			this->m_oBtnLoadLastSavedGame.SetFillColor(Entity::Color(50, 150, 50, 150));
			this->m_oBtnLoadLastSavedGame.SetHoverColor(Entity::Color(70, 180, 70, 150));
			this->m_oBtnLoadLastSavedGame.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oBtnLoadLastSavedGame.SetSize(Entity::Vector(200, 35));
			this->m_oBtnLoadLastSavedGame.SetOwner(this);
			this->m_oBtnLoadLastSavedGame.SetText(L"Quick load");

			this->m_oBtnReturnToMainMenu.SetFrameColor(Entity::Color(200, 200, 200, 150));
			this->m_oBtnReturnToMainMenu.SetFillColor(Entity::Color(50, 150, 50, 150));
			this->m_oBtnReturnToMainMenu.SetHoverColor(Entity::Color(70, 180, 70, 150));
			this->m_oBtnReturnToMainMenu.SetTextColor(Entity::Color(250, 250, 250, 255));
			this->m_oBtnReturnToMainMenu.SetSize(Entity::Vector(300, 35));
			this->m_oBtnReturnToMainMenu.SetOwner(this);
			this->m_oBtnReturnToMainMenu.SetText(L"Return to main menu");

			return true;
		}

		void SetPosition(const Entity::Vector& vec)
		{
			//Set position

			this->m_vecPos = vec;

			this->m_oForm.SetPosition(vec);
			this->m_oBtnRestart.SetPosition(Entity::Vector(vec[0] + 10, vec[1] + this->m_vecSize[1] - 115));
			this->m_oBtnLoadLastSavedGame.SetPosition(Entity::Vector(vec[0] + 10, vec[1] + this->m_vecSize[1] - 85));
			this->m_oBtnReturnToMainMenu.SetPosition(Entity::Vector(vec[0] + 10, vec[1] + this->m_vecSize[1] - 45));
		}

		void Draw(void)
		{
			//Draw menu

			this->m_oForm.Draw();
			this->m_oBtnRestart.Draw();
			this->m_oBtnLoadLastSavedGame.Draw();
			this->m_oBtnReturnToMainMenu.Draw();

			pRenderer->DrawString(pDefaultFont, L"Your game is over.", this->m_vecPos[0] + 20, this->m_vecPos[1] + 79, 250, 250, 250, 255);
			pRenderer->DrawString(pDefaultFont, L"You can try again or return to main menu.", this->m_vecPos[0] + 20, this->m_vecPos[1] + 100, 250, 250, 250, 255);
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
		{
			//Handle mouse events

			this->m_oBtnRestart.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oBtnLoadLastSavedGame.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
			this->m_oBtnReturnToMainMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
		}

		virtual void OnButtonClick(class CButton* pButton);
	};
}