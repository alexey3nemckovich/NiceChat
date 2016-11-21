// Main.cpp : Defines the entry point for the application.
#include "stdafx.h"
#include "MainWindow.h"
using namespace std;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NICECHAT));
	WindowManager::SetAppParams(hInstance, nCmdShow);
	WindowManager* windowsManager = WindowManager::GetInstance();
	windowsManager->ShowWindow(WINDOW_TYPE::MAIN);
	//Main message loop:
	MSG msg;
	int ret;
    while ((ret = GetMessage(&msg, nullptr, 0, 0)) != 0)
    {
		if (ret == -1)
		{
			return -1;
		}
		if (!IsDialogMessage(windowsManager->GetActiveWindow()->GetHWnd(), &msg))
		{
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
    }
    return (int) msg.wParam;
}