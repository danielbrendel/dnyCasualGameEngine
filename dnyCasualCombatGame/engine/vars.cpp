#include "vars.h"

DxWindow::CDxWindow* pWindow = nullptr;
DxRenderer::CDxRenderer* pRenderer = nullptr;
DxSound::CDxSound* pSound = nullptr;
Scripting::CScriptInt* pScriptingInt = nullptr;
std::wstring wszBasePath = L"";
ConfigMgr::CConfigInt* pConfigMgr = nullptr;
Console::CConsole* pConsole = nullptr;

ConfigMgr::CCVar::cvar_s* pGfxResolutionWidth = nullptr;
ConfigMgr::CCVar::cvar_s* pGfxResolutionHeight = nullptr;

DxRenderer::d3dfont_s* pDefaultFont = nullptr;