#include "shared.h"
#include "game.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Game::CGame* pGame = new Game::CGame();

	pGame->Initialize();
	pGame->Process();
	pGame->Release();

	delete pGame;

	return 0;
}