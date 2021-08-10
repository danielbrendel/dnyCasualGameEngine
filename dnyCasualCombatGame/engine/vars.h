#pragma once

/*
	Casual Combat Game (dnyCasualCombatGame) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

#include "shared.h"
#include "renderer.h"
#include "sound.h"
#include "window.h"
#include "scriptint.h"
#include "configmgr.h"
#include "console.h"

extern DxWindow::CDxWindow* pWindow;
extern DxRenderer::CDxRenderer* pRenderer;
extern DxSound::CDxSound* pSound;
extern Scripting::CScriptInt* pScriptingInt;
extern std::wstring wszBasePath;
extern ConfigMgr::CConfigInt* pConfigMgr;
extern Console::CConsole* pConsole;

extern ConfigMgr::CCVar::cvar_s* pGfxResolutionWidth;
extern ConfigMgr::CCVar::cvar_s* pGfxResolutionHeight;
extern ConfigMgr::CCVar::cvar_s* pGfxFullscreen;

extern DxRenderer::d3dfont_s* pDefaultFont;