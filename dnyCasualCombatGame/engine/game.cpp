#include "game.h"

namespace Game {
	class CGame* pGame = nullptr;
	CWindowEvents oDxWindowEvents;

	void CGame::Process(void)
	{
		while (this->m_bInit) {
			pWindow->Process();
			Entity::oScriptedEntMgr.Process();

			Sleep(1);
		}
	}

	void CGame::Draw(void)
	{
		//pConsole->Draw();

		for (size_t i = 0; i < this->m_vSolidSprites.size(); i++) {
			this->m_vSolidSprites[i].Draw();
		}

		Entity::oScriptedEntMgr.Draw();
		Entity::oScriptedEntMgr.DrawOnTop();

		if (!this->m_bGameStarted) {
			pRenderer->DrawSprite(this->m_hBanner, pGfxResolutionWidth->iValue / 2 - 768 / 2, 10, 0, 0.0f);
		}

		if (this->m_oMenu.IsOpen()) {
			this->m_oMenu.Draw();
		}
	}

	void CGame::OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		//Called for mouse events
		
		//Inform player entity
		if (!iMouseKey) {
			const Entity::CScriptedEntsMgr::playerentity_s& playerEntity = Entity::oScriptedEntMgr.GetPlayerEntity();
			Entity::Vector vPos(x, y);
			BEGIN_PARAMS(vArgs);
			PUSH_OBJECT(&vPos);
			pScriptingInt->CallScriptMethod(playerEntity.hScript, playerEntity.pObject, "void OnUpdateCursor(const Vector &in pos)", &vArgs, nullptr, Scripting::FA_VOID);
		} else {
			const Entity::CScriptedEntsMgr::playerentity_s& playerEntity = Entity::oScriptedEntMgr.GetPlayerEntity();
			BEGIN_PARAMS(vArgs);
			PUSH_DWORD(iMouseKey);
			PUSH_BYTE(bDown);
			pScriptingInt->CallScriptMethod(playerEntity.hScript, playerEntity.pObject, "void OnMousePress(int key, bool bDown)", &vArgs, nullptr, Scripting::FA_VOID);
		}

		//Pass to menu
		this->m_oMenu.OnMouseEvent(x, y, iMouseKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);
	}

	void CGame::OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		//Called for key events

		const Entity::CScriptedEntsMgr::playerentity_s& playerEntity = Entity::oScriptedEntMgr.GetPlayerEntity();
		BEGIN_PARAMS(vArgs);
		PUSH_DWORD(vKey);
		PUSH_BYTE(bDown);
		std::cout << "KEYEVENT" << std::endl;
		pScriptingInt->CallScriptMethod(playerEntity.hScript, playerEntity.pObject, "void OnKeyPress(int vKey, bool bDown)", &vArgs, nullptr, Scripting::FA_VOID);
	}

	void CWindowEvents::OnCreated(HWND hWnd)
	{
	}

	LRESULT CWindowEvents::OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld)
	{
		//Handle key input

		if (vKey == VK_F1) {
			pConsole->Toggle();
		}

		pGame->OnKeyEvent(vKey, bDown, bCtrlHeld, bShiftHeld, bAltHeld);

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

	void AS_MessageCallback(const asSMessageInfo* msg, void* param)
	{
		//Callback function for AngelScript message outputs

		Console::ConColor sColor(200, 200, 200);

		std::string szMsgType("Info");
		if (msg->type == asMSGTYPE_WARNING) {
			szMsgType = "Warning";
			sColor = Console::ConColor(150, 150, 0);
		}
		else if (msg->type == asMSGTYPE_ERROR) {
			szMsgType = "Error";
			sColor = Console::ConColor(250, 0, 0);
		}

		std::string szErrMsg = "[AngelScript][" + szMsgType + "] " + std::string(msg->section) + " (" + std::to_string(msg->row) + ":" + std::to_string(msg->col) + "): " + msg->message + "\n";

		pConsole->AddLine(Utils::ConvertToWideString(szErrMsg), sColor);
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

	void Cmd_EntSpawn(void)
	{
		std::wstring wszEntityName = pConfigMgr->ExpressionItemValue(1);
		int x = _wtoi(pConfigMgr->ExpressionItemValue(2).c_str());
		int y = _wtoi(pConfigMgr->ExpressionItemValue(3).c_str());

		pGame->SpawnEntity(wszEntityName, x, y);
	}
}