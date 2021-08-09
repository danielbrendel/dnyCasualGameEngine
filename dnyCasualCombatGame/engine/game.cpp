#include "game.h"

namespace Game {
	class CGame* pGame = nullptr;
	CWindowEvents oDxWindowEvents;

	void CWindowEvents::OnCreated(HWND hWnd)
	{
	}

	LRESULT CWindowEvents::OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		//Handle key input

		if (vKey == VK_F1) {
			pConsole->Toggle();
		}

		return 0;
	}

	LRESULT CWindowEvents::OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		//Handle mouse input

		pGame->OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

		return 0;
	}

	void CWindowEvents::OnMouseWheel(short wDistance, bool bForward)
	{
	}

	void CWindowEvents::OnDraw(void)
	{
		pRenderer->DrawBegin();
		pGame->Draw();
		pRenderer->DrawEnd();
	}

	void CWindowEvents::OnRelease(void)
	{
	}

	void UnknownExpressionHandler(const std::wstring& szCmdName)
	{
		//Handler for unknown expressions

		pConsole->AddLine(L"Unknown command: \"" + szCmdName + L"\"");
	}
}