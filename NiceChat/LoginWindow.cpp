#include "stdafx.h"
#include "LoginWindow.h"


HWND LoginWindow::hPassText;
HWND LoginWindow::hPassEdit;
HWND LoginWindow::hLoginText;
HWND LoginWindow::hLoginEdit;
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
	int w = 150;
	int h = 50;
	int loginBtnWidth = 200;
	int loginBtnHeight = 50;
	DWORD staticStyle = SS_CENTER | WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD editStyle = ES_CENTER | WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD editPassStyle = ES_PASSWORD | editStyle;
	DWORD btnStyle;
	hLoginText = windowConstructor->CreateControl(L"STATIC", L"Login", hWnd, 100, 100, w, h, staticStyle);
	hLoginEdit = windowConstructor->CreateControl(L"EDIT", L"Login", hWnd, 350, 100, w, h, editStyle);
	hLoginText = windowConstructor->CreateControl(L"STATIC", L"Password", hWnd, 100, 200, w, h, staticStyle);
	hLoginEdit = windowConstructor->CreateControl(L"EDIT", L"Password", hWnd, 350, 200, w, h, editPassStyle);
	hLoginBtn = windowConstructor->CreateControl(L"BUTTON", L"Login",hWnd, wndCenter.x - loginBtnWidth / 2, 275, loginBtnWidth, loginBtnHeight);
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