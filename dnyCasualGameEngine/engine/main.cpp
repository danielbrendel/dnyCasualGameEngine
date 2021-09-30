#include "shared.h"
#include "game.h"

/*
	Casual Game Engine (dnyCasualGameEngine) developed by Daniel Brendel

	(C) 2021 by Daniel Brendel

	Contact: dbrendel1988<at>gmail<dot>com
	GitHub: https://github.com/danielbrendel/

	Released under the MIT license
*/

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game::CGame* pGame = new Game::CGame();
	
	std::wstring wszArgs = Utils::ConvertToWideString(lpCmdLine);
	if (wszArgs.find(L"-workshop_item_update") != -1) {
		Game::HandlePackageUpload(wszArgs);

		return 0;
	}

	pGame->Initialize();
	pGame->Process();
	pGame->Release();

	delete pGame;

	return 0;
}