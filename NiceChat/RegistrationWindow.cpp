#include "stdafx.h"
#include "RegistrationWindow.h"


RegistrationWindow::RegistrationWindow() 
	: Window(RegistrationWindow::RegistrationProc, _T("RegWindowClass"), _T("Registration"), 400, 600)
{
	SetMenu(hWnd, LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(ID_REGISTRATION_MENU)));
	Init();
}


void RegistrationWindow::Init()
{

}


RegistrationWindow::~RegistrationWindow()
{

}


LRESULT CALLBACK RegistrationWindow::RegistrationProc(
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
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_R_ABOUT:
			dialogManager->ShowDialog(DIALOG_TYPE::ABOUT);
			break;
		case ID_R_LOGIN:
			windowsManager->ShowWindow(WINDOW_TYPE::LOGIN);
			break;
		case ID_R_MAIN:
			windowsManager->ShowWindow(WINDOW_TYPE::MAIN);
			break;
		case ID_R_EXIT:
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