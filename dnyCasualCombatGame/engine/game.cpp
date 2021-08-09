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

	void Cmd_PackageName(void)
	{
		pGame->m_sPackage.wszName = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_PackageVersion(void)
	{
		pGame->m_sPackage.wszVersion = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_PackageAuthor(void)
	{
		pGame->m_sPackage.wszAuthor = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_PackageContact(void)
	{
		pGame->m_sPackage.wszContact = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_MapIndex(void)
	{
		pGame->m_sPackage.wszMapIndex = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_MapName(void)
	{
		pGame->m_sMap.wszName = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_MapBackground(void)
	{
		pGame->m_sMap.wszBackground = pConfigMgr->ExpressionItemValue(1);
	}

	void Cmd_PlayerSpawn(void)
	{
		int x = _wtoi(pConfigMgr->ExpressionItemValue(1).c_str());
		int y = _wtoi(pConfigMgr->ExpressionItemValue(2).c_str());

		pGame->m_sPlayerSpawn.pos = Entity::Vector(x, y);
	}

	void Cmd_EnvSolidSprite(void)
	{
		std::wstring wszFile = pConfigMgr->ExpressionItemValue(1);
		int x = _wtoi(pConfigMgr->ExpressionItemValue(2).c_str());
		int y = _wtoi(pConfigMgr->ExpressionItemValue(3).c_str());
		int w = _wtoi(pConfigMgr->ExpressionItemValue(4).c_str());
		int h = _wtoi(pConfigMgr->ExpressionItemValue(5).c_str());
		float rot = _wtof(pConfigMgr->ExpressionItemValue(6).c_str());
		int repeat = _wtoi(pConfigMgr->ExpressionItemValue(7).c_str());
		int dir = _wtoi(pConfigMgr->ExpressionItemValue(8).c_str());

		Entity::CSolidSprite oSprite;
		oSprite.Initialize(x, y, w, h, wszBasePath + L"packages\\" + pGame->m_sPackage.wszPakName + L"\\gfx\\" + wszFile, repeat, dir, rot);

		pGame->m_vSolidSprites.push_back(oSprite);
	}
}