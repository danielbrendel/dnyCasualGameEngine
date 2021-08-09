#include "window.h"

namespace DxWindow {
	class CDxWindow* pDxWindowInstance = nullptr;

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return pDxWindowInstance->WndProc(hWnd, uMsg, wParam, lParam);
	}
}