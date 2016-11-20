#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() 
	: Window(MainWndProc, _T("MainWindowClass"), _T("NiceChat"), 800, 600)
{
	SetMenu(hWnd, LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(IDC_NICECHAT)));
	Init();
}


void MainWindow::Init()
{

}


MainWindow::~MainWindow()
{

}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		static MainWindow* mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::REGISTRATION);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_M_ABOUT:
			mainWindow->dialogManager->ShowDialog(DIALOG_TYPE::ABOUT);
			break;
		case ID_M_LOGIN:
			mainWindow->windowManager->ShowWindow(WINDOW_TYPE::LOGIN);
			break;
		case ID_M_REGISTRATE:
			mainWindow->windowManager->ShowWindow(WINDOW_TYPE::REGISTRATION);
			break;
		case ID_M_LEAVE_CHAT:
			break;
		case ID_M_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}