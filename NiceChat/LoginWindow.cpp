#include "stdafx.h"
#include "LoginWindow.h"


HWND LoginWindow::hLoginBtn;


LoginWindow::LoginWindow() 
	: Window(LoginWindow::LoginProc, _T("LogWindowClass"), _T("Login"), 600, 400)
{
	SetMenu(hWnd, LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(ID_LOGIN_MENU)));
	Init();
}


void LoginWindow::Init()
{
	POINT wndCenter = Window::GetHWNDCenter(hWnd);
	int width = 100;
	int height = 100;
	hLoginBtn = windowConstructor->CreateControl(L"BUTTON", L"Login",hWnd, wndCenter.x - width / 2, wndCenter.y - height /2 , width, height);
}


LoginWindow::~LoginWindow()
{

}


LRESULT CALLBACK LoginWindow::LoginProc(
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
		case 0:
			BtnClick(lParam);
			break;
		case ID_L_ABOUT:
			dialogManager->ShowDialog(DIALOG_TYPE::ABOUT);
			break;
		case ID_L_REGISTRATE:
			windowsManager->ShowWindow(WINDOW_TYPE::REGISTRATION);
			break;
		case ID_L_MAIN:
			windowsManager->ShowWindow(WINDOW_TYPE::MAIN);
			break;
		case ID_L_EXIT:
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


void LoginWindow::BtnClick(LPARAM lParam)
{
	HWND hBtn = (HWND)lParam;
	if (hBtn == hLoginBtn)
	{
		Beep(500, 100);
	}
}