#pragma once

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

extern DxRenderer::d3dfont_s* pDefaultFont;