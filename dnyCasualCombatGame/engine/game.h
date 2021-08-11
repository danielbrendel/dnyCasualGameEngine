#pragma once

/*
	Casual Combat Game (dnyCasualCombatGame) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "renderer.h"
#include "sound.h"
#include "window.h"
#include "vars.h"
#include "entity.h"
#include "menu.h"
#include "input.h"

/* Game specific environment */
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
	void Cmd_EntRequire(void);
	void Cmd_EntSpawn(void);
	void Cmd_EnvGoal(void);
	void Cmd_Bind(void);

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
		Entity::CGoalEntity* m_pGoalEntity;
		Input::CInputMgr m_oInputMgr;

		friend void Cmd_PackageName(void);
		friend void Cmd_PackageVersion(void);
		friend void Cmd_PackageAuthor(void);
		friend void Cmd_PackageContact(void);
		friend void Cmd_MapIndex(void);
		friend void Cmd_MapName(void);
		friend void Cmd_MapBackground(void);
		friend void Cmd_EnvSolidSprite(void);
		friend void Cmd_EntSpawn(void);
		friend void Cmd_EntRequire(void);
		friend void Cmd_EnvGoal(void);
		friend void Cmd_Bind(void);

		bool LoadPackage(const std::wstring& wszPackage)
		{
			//Load package game

			this->m_sPackage.wszPakName = wszPackage;
			
			//Check if package folder exists
			if (!Utils::DirExists(wszBasePath + L"packages\\" + wszPackage)) {
				pConsole->AddLine(L"Package not found: " + wszPackage, Console::ConColor(255, 0, 0));
				return false;
			}
			
			//Check if package index config exists
			if (!Utils::FileExists(wszBasePath + L"packages\\" + wszPackage + L"\\" + wszPackage + L".cfg")) {
				pConsole->AddLine(L"Package configuration script not found", Console::ConColor(255, 0, 0));
				return false;
			}

			//Execute package index config
			if (!pConfigMgr->Execute(wszBasePath + L"packages\\" + wszPackage + L"\\" + wszPackage + L".cfg")) {
				pConsole->AddLine(L"Failed to execute package configuration script", Console::ConColor(255, 0, 0));
				return false;
			}

			pConsole->AddLine(L"Package " + this->m_sPackage.wszName + L" v" + this->m_sPackage.wszVersion + L" by " + this->m_sPackage.wszAuthor + L" (" + this->m_sPackage.wszContact + L")");

			//Execute package index map file
			if (!pConfigMgr->Execute(wszBasePath + L"packages\\" + wszPackage + L"\\maps\\" + this->m_sPackage.wszMapIndex)) {
				pConsole->AddLine(L"Failed to execute package index map script", Console::ConColor(255, 0, 0));
				return false;
			}

			//Set map background
			pRenderer->SetBackgroundPicture(wszBasePath + L"\\packages\\" + wszPackage + L"\\gfx\\" + this->m_sMap.wszBackground);

			this->m_bGameStarted = true;

			return this->m_bGameStarted;
		}

		bool LoadMap(const std::wstring& wszMap);

		bool SpawnEntity(const std::wstring& wszName, int x, int y, float rot)
		{
			//Spawn entity into world

			//Check if entity script exists
			if (!Utils::FileExists(wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as")) {
				pConsole->AddLine(L"Entity script does not exist", Console::ConColor(255, 0, 0));
				return false;
			}

			//Load entity script if not already loaded
			Scripting::HSISCRIPT hScript;
			size_t uiScriptListId = this->FindScript(wszName);
			if (uiScriptListId == std::string::npos) {
				hScript = pScriptingInt->LoadScript(Utils::ConvertToAnsiString(wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as"));
				if (hScript == SI_INVALID_ID) {
					pConsole->AddLine(L"Failed to load entity script: " + wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as", Console::ConColor(255, 0, 0));
					return false;
				}

				//Store data
				entityscript_s sEntScript;
				sEntScript.hScript = hScript;
				sEntScript.wszIdent = wszName;
				this->m_vEntityScripts.push_back(sEntScript);
				uiScriptListId = this->m_vEntityScripts.size() - 1;
			} else {
				hScript = this->m_vEntityScripts[uiScriptListId].hScript;
			}

			//Call OnSpawn() script function to let the entity be created

			Entity::Vector vecPos(x, y);
			std::string szIdent = Utils::ConvertToAnsiString(wszName);
			std::string szPath = Utils::ConvertToAnsiString(wszBasePath) + "packages\\" + Utils::ConvertToAnsiString(this->m_sPackage.wszPakName) + "\\";

			BEGIN_PARAMS(vArgs);
			PUSH_OBJECT(&vecPos);
			PUSH_FLOAT(rot);
			PUSH_OBJECT(&szIdent);
			PUSH_OBJECT(&szPath);
			
			bool bResult = pScriptingInt->CallScriptFunction(hScript, true, "OnSpawn", &vArgs, nullptr, Scripting::FA_VOID);

			END_PARAMS(vArgs);

			if (!bResult) {
				pConsole->AddLine(L"Failed to call OnSpawn() in script", Console::ConColor(255, 0, 0));
				pScriptingInt->UnloadScript(hScript);
				this->m_vEntityScripts.erase(this->m_vEntityScripts.begin() + uiScriptListId);
				return false;
			}

			return true;
		}

		bool RequireEntityScript(const std::wstring& wszName)
		{
			//Load entity script
			// 
			//Check if entity script exists
			if (!Utils::FileExists(wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as")) {
				pConsole->AddLine(L"Entity script does not exist", Console::ConColor(255, 0, 0));
				return false;
			}

			//Load entity script if not already loaded
			Scripting::HSISCRIPT hScript;
			size_t uiScriptListId = this->FindScript(wszName);
			if (uiScriptListId == std::string::npos) {
				hScript = pScriptingInt->LoadScript(Utils::ConvertToAnsiString(wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as"));
				if (hScript == SI_INVALID_ID) {
					pConsole->AddLine(L"Failed to load entity script: " + wszBasePath + L"\\packages\\" + this->m_sPackage.wszPakName + L"\\entities\\" + wszName + L".as", Console::ConColor(255, 0, 0));
					return false;
				}

				//Store data
				entityscript_s sEntScript;
				sEntScript.hScript = hScript;
				sEntScript.wszIdent = wszName;
				this->m_vEntityScripts.push_back(sEntScript);
			}

			return true;
		}

		void SpawnGoal(int x, int y, const std::wstring& wszGoal)
		{
			//Spawn goal entity

			//Free previous goal entity if was spawned
			if (this->m_pGoalEntity) {
				delete this->m_pGoalEntity;
				this->m_pGoalEntity = nullptr;
			}

			this->m_pGoalEntity = new Entity::CGoalEntity();
			this->m_pGoalEntity->SetPosition(x, y);
			this->m_pGoalEntity->SetGoal(wszGoal);
		}

		size_t FindScript(const std::wstring& wszName)
		{
			//Find script by name if loaded

			for (size_t i = 0; i < this->m_vEntityScripts.size(); i++) {
				if (this->m_vEntityScripts[i].wszIdent == wszName) {
					return i;
				}
			}

			return std::string::npos;
		}
	public:
		CGame() : m_bInit(false), m_bGameStarted(false) { pGame = this; }
		~CGame() { pGame = nullptr; }

		bool Initialize(void)
		{
			//Initialize game

			if (this->m_bInit) {
				return true;
			}

			//Get base game path

			wchar_t wszAppPath[2080];
			GetModuleFileName(0, wszAppPath, sizeof(wszAppPath));
			for (size_t i = wcslen(wszAppPath); i >= 0; i--) {
				if (wszAppPath[i] == '\\') {
					break;
				}

				wszAppPath[i] = 0;
			}
			
			wszBasePath = wszAppPath;
			
			//Initialize config manager

			pConfigMgr = new ConfigMgr::CConfigInt();
			if (!pConfigMgr) {
				this->Release();
				return false;
			}
			
			pConfigMgr->SetUnknownExpressionInformer(&UnknownExpressionHandler);

			//Add CVars
			pGfxResolutionWidth = pConfigMgr->CCVar::Add(L"gfx_resolution_width", ConfigMgr::CCVar::CVAR_TYPE_INT, L"1024");
			pGfxResolutionHeight = pConfigMgr->CCVar::Add(L"gfx_resolution_height", ConfigMgr::CCVar::CVAR_TYPE_INT, L"768");
			pGfxFullscreen = pConfigMgr->CCVar::Add(L"gfx_fullscreen", ConfigMgr::CCVar::CVAR_TYPE_BOOL, L"1");
			
			//Add commands
			pConfigMgr->CCommand::Add(L"bind", L"Bind command to key", &Cmd_Bind);
			pConfigMgr->CCommand::Add(L"package_name", L"Package name", &Cmd_PackageName);
			pConfigMgr->CCommand::Add(L"package_version", L"Package version", &Cmd_PackageVersion);
			pConfigMgr->CCommand::Add(L"package_author", L"Package author", &Cmd_PackageAuthor);
			pConfigMgr->CCommand::Add(L"package_contact", L"Package contact information", &Cmd_PackageContact);
			pConfigMgr->CCommand::Add(L"map_index", L"Map to begin game with", &Cmd_MapIndex);
			pConfigMgr->CCommand::Add(L"map_name", L"Map name", &Cmd_MapName);
			pConfigMgr->CCommand::Add(L"map_background", L"Map background", &Cmd_MapBackground);
			pConfigMgr->CCommand::Add(L"env_solidsprite", L"Solid sprite placement", &Cmd_EnvSolidSprite);
			pConfigMgr->CCommand::Add(L"ent_spawn", L"Spawn scripted entity", &Cmd_EntSpawn);
			pConfigMgr->CCommand::Add(L"ent_require", L"Require entity script", &Cmd_EntRequire);
			pConfigMgr->CCommand::Add(L"env_goal", L"Spawn goal entity", &Cmd_EnvGoal);

			//Execute game config script
			pConfigMgr->Execute(wszBasePath + L"config.cfg");
			
			//Instantiate window manager
			pWindow = new DxWindow::CDxWindow();
			if (!pWindow) {
				this->Release();
				return false;
			}
			
			//Instantiate renderer
			pRenderer = new DxRenderer::CDxRenderer();
			if (!pRenderer) {
				this->Release();
				return false;
			}

			//Instantiate sound manager
			pSound = new DxSound::CDxSound();
			if (!pSound) {
				this->Release();
				return false;
			}
			
			//Initialize game window
			if (!pWindow->Initialize(APP_NAME, pGfxResolutionWidth->iValue, pGfxResolutionHeight->iValue, &oDxWindowEvents)) {
				this->Release();
				return false;
			}
			
			//Initialize renderer
			if (!pRenderer->Initialize(pWindow->GetHandle(), !pGfxFullscreen->bValue, pWindow->GetResolutionX(), pWindow->GetResolutionY(), 0, 0, 0, 255)) {
				this->Release();
				return false;
			}

			//Initialize sound
			if (!pSound->Initialize(pWindow->GetHandle())) {
				this->Release();
				return false;
			}

			//Instantiate console manager
			pConsole = new Console::CConsole();
			if (!pConsole) {
				this->Release();
				return false;
			}
			
			//Initialize console
			if (!pConsole->Initialize(pRenderer, pGfxResolutionWidth->iValue, 500, 50, Console::ConColor(100, 100, 50))) {
				this->Release();
				return false;
			}

			//Instantiate and initialize AngelScript scripting interface
			pScriptingInt = new Scripting::CScriptInt("", &AS_MessageCallback);
			if (!pScriptingInt) {
				this->Release();
				return false;
			}
			
			//Initialize entity environment
			if (!Entity::Initialize()) {
				this->Release();
				return false;
			}
			
			//Initialize menu
			if (!m_oMenu.Initialize(pGfxResolutionWidth->iValue, pGfxResolutionHeight->iValue)) {
				this->Release();
				return false;
			}

			//Load banner
			this->m_hBanner = pRenderer->LoadSprite(wszBasePath + L"media\\banner.png", 1, 768, 150, 1, false);
			
			//Set game menu background and open the menu
			pRenderer->SetBackgroundPicture(wszBasePath + L"media\\background.jpg");
			this->m_oMenu.SetOpenStatus(true);

			//Add info text
			pConsole->AddLine(APP_NAME L" v" APP_VERSION L" developed by " APP_AUTHOR L" (" APP_CONTACT L")", Console::ConColor(100, 215, 255));
			pConsole->AddLine(L"");

			this->m_bInit = true;
			this->m_bGameStarted = false;
			this->StartGame(L"test");
			return this->m_bInit;
		}

		bool StartGame(const std::wstring& wszPackage)
		{
			//Start a new game from package

			pConsole->AddLine(L"Starting new game using package \"" + wszPackage + L"\"...");

			//Stop current game if running any
			if (this->m_bGameStarted) {
				this->StopGame();
				pConsole->AddLine(L"Stopped previous game");
			}

			if (!this->m_bInit) {
				return false;
			}

			//Load package
			return this->LoadPackage(wszPackage);
		}

		void StopGame(void);

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

		bool IsVectorFieldInsideWall(const Entity::Vector& vecPos, const Entity::Vector& vecSize)
		{
			//Check if vector is inside wall

			for (size_t i = 0; i < this->m_vSolidSprites.size(); i++) {
				if (!this->m_vSolidSprites[i].IsWall()) {
					continue;
				}

				if (this->m_vSolidSprites[i].IsVectorFieldCollided(vecPos, vecSize)) {
					return true;
				}
			}

			return false;
		}

		void OnMouseEvent(int x, int y, int iMouseKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);
		void OnKeyEvent(int vKey, bool bDown, bool bCtrlHeld, bool bShiftHeld, bool bAltHeld);

		void Release(void)
		{
			//Release game component

			//Stop current game
			this->StopGame();

			//Free memory
			FREE(pConsole);
			FREE(pSound);
			FREE(pRenderer);
			FREE(pWindow);
			FREE(pScriptingInt);
			FREE(pConfigMgr);

			//Clear indicators
			this->m_bGameStarted = false;
			this->m_bInit = false;
		}

		//Return package path
		std::wstring GetPackagePath(void) { return wszBasePath + L"packages\\" + this->m_sPackage.wszPakName + L"\\"; }
		//Return key binding
		int GetKeyBinding(const std::wstring& wszIdent) { return this->m_oInputMgr.GetKeyBindingCode(wszIdent); }
	};
}