#pragma once

#include "renderer.h"
#include "sound.h"
#include "window.h"
#include "vars.h"
#include "entity.h"
#include "menu.h"

namespace Game {
	extern class CGame* pGame;

	//Window event handler component
	struct CWindowEvents : public DxWindow::CDxWindow::IWindowEvents {
		virtual void OnCreated(HWND hWnd);
		virtual LRESULT OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);
		virtual LRESULT OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);
		virtual void OnMouseWheel(short wDistance, bool bForward);
		virtual void OnDraw(void);
		virtual void OnRelease(void);
	};

	extern CWindowEvents oDxWindowEvents;

	void UnknownExpressionHandler(const std::wstring& szCmdName);
	void AS_MessageCallback(const asSMessageInfo* msg, void* param);

	void Cmd_PackageName(void);
	void Cmd_PackageVersion(void);
	void Cmd_PackageAuthor(void);
	void Cmd_PackageContact(void);
	void Cmd_MapIndex(void);
	void Cmd_MapName(void);
	void Cmd_MapBackground(void);
	void Cmd_EnvSolidSprite(void);
	void Cmd_EntSpawn(void);

	class CGame {
	private:
		struct package_s {
			std::wstring wszPakName;
			std::wstring wszName;
			std::wstring wszVersion;
			std::wstring wszAuthor;
			std::wstring wszContact;
			std::wstring wszMapIndex;
		};

		struct map_s {
			std::wstring wszName;
			std::wstring wszBackground;
		};

		struct player_s {
			Entity::Vector pos;
		};

		struct entityscript_s {
			Scripting::HSISCRIPT hScript;
			std::wstring wszIdent;
		};

		bool m_bInit;
		bool m_bGameStarted;
		DxRenderer::HD3DSPRITE m_hBanner;
		Menu::CMenu m_oMenu;
		package_s m_sPackage;
		map_s m_sMap;
		player_s m_sPlayerSpawn;
		std::vector<Entity::CSolidSprite> m_vSolidSprites;
		std::vector<entityscript_s> m_vEntityScripts;

		friend void Cmd_PackageName(void);
		friend void Cmd_PackageVersion(void);
		friend void Cmd_PackageAuthor(void);
		friend void Cmd_PackageContact(void);
		friend void Cmd_MapIndex(void);
		friend void Cmd_MapName(void);
		friend void Cmd_MapBackground(void);
		friend void Cmd_EnvSolidSprite(void);
		friend void Cmd_EntSpawn(void);

		bool LoadPackage(const std::wstring& wszPackage)
		{
			//Load package game

			this->m_sPackage.wszPakName = wszPackage;

			if (!Utils::DirExists(wszBasePath + L"packages\\" + wszPackage)) {
				pConsole->AddLine(L"Package not found: " + wszPackage);
				return false;
			}
			std::cout << Utils::ConvertToAnsiString(wszBasePath + L"packages\\" + wszPackage + L"\\" + wszPackage + L".cfg") <<std::endl;
			if (!Utils::FileExists(wszBasePath + L"packages\\" + wszPackage + L"\\" + wszPackage + L".cfg")) {
				pConsole->AddLine(L"Package configuration script not found");
				return false;
			}

			if (!pConfigMgr->Execute(wszBasePath + L"packages\\" + wszPackage + L"\\" + wszPackage + L".cfg")) {
				pConsole->AddLine(L"Failed to execute package configuration script");
				return false;
			}

			pConsole->AddLine(L"Package " + this->m_sPackage.wszName + L" v" + this->m_sPackage.wszVersion + L" by " + this->m_sPackage.wszAuthor + L" (" + this->m_sPackage.wszContact + L")");

			if (!pConfigMgr->Execute(wszBasePath + L"packages\\" + wszPackage + L"\\maps\\" + this->m_sPackage.wszMapIndex)) {
				pConsole->AddLine(L"Failed to execute package index map script");
				return false;
			}

			pRenderer->SetBackgroundPicture(wszBasePath + L"\\packages\\" + wszPackage + L"\\gfx\\" + this->m_sMap.wszBackground);

			this->m_bGameStarted = true;

			return this->m_bGameStarted;
		}

		bool SpawnEntity(const std::wstring& wszName, int x, int y)
		{
			//Spawn entity into world

			if (!Utils::FileExists(wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as")) {
				pConsole->AddLine(L"Entity script does not exist");
				return false;
			}

			Scripting::HSISCRIPT hScript = pScriptingInt->LoadScript(Utils::ConvertToAnsiString(wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as"));
			if (hScript == SI_INVALID_ID) {
				pConsole->AddLine(L"Failed to load entity script: " + wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as");
				return false;
			}

			entityscript_s sEntScript;
			sEntScript.hScript = hScript;
			sEntScript.wszIdent = wszName;
			this->m_vEntityScripts.push_back(sEntScript);

			Entity::Vector vecPos(x, y);
			std::string szIdent = Utils::ConvertToAnsiString(wszName);
			std::string szPath = Utils::ConvertToAnsiString(wszBasePath) + "packages\\" + Utils::ConvertToAnsiString(this->m_sPackage.wszPakName) + "\\";

			BEGIN_PARAMS(vArgs);
			PUSH_OBJECT(&vecPos);
			PUSH_OBJECT(&szIdent);
			PUSH_OBJECT(&szPath);
			
			bool bResult = pScriptingInt->CallScriptFunction(hScript, true, "OnSpawn", &vArgs, nullptr, Scripting::FA_VOID);

			END_PARAMS(vArgs);

			if (!bResult) {
				pConsole->AddLine(L"Failed to call OnSpawn() in script");
				pScriptingInt->UnloadScript(hScript);
				this->m_vEntityScripts.erase(this->m_vEntityScripts.begin() + this->m_vEntityScripts.size() - 1);
				return false;
			}

			return true;
		}
	public:
		CGame() : m_bInit(false), m_bGameStarted(false) { pGame = this; }
		~CGame() { pGame = nullptr; }

		bool Initialize(void)
		{
			AllocConsole();
			FILE* fDummy;
			freopen_s(&fDummy, "CONIN$", "r", stdin);
			freopen_s(&fDummy, "CONOUT$", "w", stderr);
			freopen_s(&fDummy, "CONOUT$", "w", stdout);
			if (this->m_bInit) {
				return true;
			}

			wchar_t wszAppPath[2080];
			GetModuleFileName(0, wszAppPath, sizeof(wszAppPath));
			for (size_t i = wcslen(wszAppPath); i >= 0; i--) {
				if (wszAppPath[i] == '\\') {
					break;
				}

				wszAppPath[i] = 0;
			}
			
			wszBasePath = wszAppPath;
			
			pConfigMgr = new ConfigMgr::CConfigInt();
			if (!pConfigMgr) {
				this->Release();
				return false;
			}

			pConfigMgr->SetUnknownExpressionInformer(&UnknownExpressionHandler);

			pGfxResolutionWidth = pConfigMgr->CCVar::Add(L"gfx_resolution_width", ConfigMgr::CCVar::CVAR_TYPE_INT, L"1024");
			pGfxResolutionHeight = pConfigMgr->CCVar::Add(L"gfx_resolution_height", ConfigMgr::CCVar::CVAR_TYPE_INT, L"768");

			pConfigMgr->CCommand::Add(L"package_name", L"Package name", &Cmd_PackageName);
			pConfigMgr->CCommand::Add(L"package_version", L"Package version", &Cmd_PackageVersion);
			pConfigMgr->CCommand::Add(L"package_author", L"Package author", &Cmd_PackageAuthor);
			pConfigMgr->CCommand::Add(L"package_contact", L"Package contact information", &Cmd_PackageContact);
			pConfigMgr->CCommand::Add(L"map_index", L"Map to begin game with", &Cmd_MapIndex);
			pConfigMgr->CCommand::Add(L"map_name", L"Map name", &Cmd_MapName);
			pConfigMgr->CCommand::Add(L"map_background", L"Map background", &Cmd_MapBackground);
			pConfigMgr->CCommand::Add(L"env_solidsprite", L"Solid sprite placement", &Cmd_EnvSolidSprite);
			pConfigMgr->CCommand::Add(L"ent_spawn", L"Spawn scripted entity", &Cmd_EntSpawn);

			pConfigMgr->Execute(wszBasePath + L"config.cfg");

			pWindow = new DxWindow::CDxWindow();
			if (!pWindow) {
				this->Release();
				return false;
			}
			
			pRenderer = new DxRenderer::CDxRenderer();
			if (!pRenderer) {
				this->Release();
				return false;
			}

			pSound = new DxSound::CDxSound();
			if (!pSound) {
				this->Release();
				return false;
			}
			
			if (!pWindow->Initialize(APP_NAME, pGfxResolutionWidth->iValue, pGfxResolutionHeight->iValue, &oDxWindowEvents)) {
				this->Release();
				return false;
			}
			
			if (!pRenderer->Initialize(pWindow->GetHandle(), true, pGfxResolutionWidth->iValue, pGfxResolutionHeight->iValue, 0, 0, 0, 255)) {
				this->Release();
				return false;
			}

			if (!pSound->Initialize(pWindow->GetHandle())) {
				this->Release();
				return false;
			}

			pConsole = new Console::CConsole();
			if (!pConsole) {
				this->Release();
				return false;
			}
			
			if (!pConsole->Initialize(pRenderer, pGfxResolutionWidth->iValue, 500, 50, Console::ConColor(100, 100, 50))) {
				this->Release();
				return false;
			}

			pScriptingInt = new Scripting::CScriptInt("", &AS_MessageCallback);
			if (!pScriptingInt) {
				this->Release();
				return false;
			}

			if (!Entity::Initialize()) {
				this->Release();
				return false;
			}
			
			if (!m_oMenu.Initialize(pGfxResolutionWidth->iValue, pGfxResolutionHeight->iValue)) {
				this->Release();
				return false;
			}

			this->m_hBanner = pRenderer->LoadSprite(wszBasePath + L"media\\banner.png", 1, 768, 150, 1, false);
			
			pRenderer->SetBackgroundPicture(wszBasePath + L"media\\background.jpg");
			this->m_oMenu.SetOpenStatus(true);

			this->m_bInit = true;
			this->m_bGameStarted = false;
			this->StartGame(L"test");
			return this->m_bInit;
		}

		bool StartGame(const std::wstring& wszPackage)
		{
			pConsole->AddLine(L"Starting new game using package \"" + wszPackage + L"\"...");

			if (this->m_bGameStarted) {
				this->StopGame();
				pConsole->AddLine(L"Stopped previous game");
			}

			if (!this->m_bInit) {
				return false;
			}

			return this->LoadPackage(wszPackage);
		}

		void StopGame(void)
		{
			//Stop game and cleanup

			//Unload entity scripts
			for (size_t i = 0; i < this->m_vEntityScripts.size(); i++) {
				pScriptingInt->UnloadScript(this->m_vEntityScripts[i].hScript);
			}

			//Release solid sprites
			for (size_t i = 0; i < this->m_vSolidSprites.size(); i++) {
				this->m_vSolidSprites[i].Release();
			}

			//Clear lists
			this->m_vEntityScripts.clear();
			this->m_vSolidSprites.clear();

			//Reset indicator
			this->m_bGameStarted = false;
		}

		void Process(void);
		void Draw(void);

		Scripting::HSISCRIPT GetScriptHandleByIdent(const std::wstring& wszIdent)
		{
			//Get script handle by ident

			for (size_t i = 0; i < this->m_vEntityScripts.size(); i++) {
				if (this->m_vEntityScripts[i].wszIdent == wszIdent) {
					return this->m_vEntityScripts[i].hScript;
				}
			}

			return SI_INVALID_ID;
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);
		void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);

		void Release(void)
		{
			this->StopGame();

			FREE(pConsole);
			FREE(pSound);
			FREE(pRenderer);
			FREE(pWindow);
			FREE(pScriptingInt);
			FREE(pConfigMgr);

			this->m_bGameStarted = false;
			this->m_bInit = false;
		}
	};


}