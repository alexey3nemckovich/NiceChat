#include "stdafx.h"
#include "LoginWindow.h"


LoginWindow::LoginWindow() 
	: Window(LoginProc, _T("LogWindowClass"), _T("Login"), 600, 400)
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
	DWORD staticStyle = SS_CENTER | WS_BORDER | WS_VISIBLE | WS_CHILD;
	DWORD editStyle = ES_CENTER | WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD editPassStyle = ES_PASSWORD | editStyle;
	DWORD btnStyle;
	hLoginText = windowConstructor->CreateControl(L"STATIC", L"Login", hWnd, 100, 100, w, h, staticStyle);
	hLoginEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 350, 100, w, h, editStyle);
	hPassText = windowConstructor->CreateControl(L"STATIC", L"Password", hWnd, 100, 200, w, h, staticStyle);
	hPassEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 350, 200, w, h, editPassStyle);
	hLoginBtn = windowConstructor->CreateControl(L"BUTTON", L"Login",hWnd, wndCenter.x - loginBtnWidth / 2, 275, loginBtnWidth, loginBtnHeight);
	//INIT FIELDS
	login = (char*)malloc(strBuffSize);
	pass = (char*)malloc(strBuffSize);
}


LoginWindow::~LoginWindow()
{
	free(login);
	free(pass);
}


LRESULT CALLBACK LoginProc(
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
		static LoginWindow* loginWindow = (LoginWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::LOGIN);
		// Parse the menu selections:
		switch (wmId)
		{
		case 0:
			loginWindow->InnerControlsProc(lParam, HIWORD(wParam));
			break;
		case ID_L_ABOUT:
			loginWindow->dialogManager->ShowDialog(DIALOG_TYPE::ABOUT);
			break;
		case ID_L_REGISTRATE:
			loginWindow->windowManager->ShowWindow(WINDOW_TYPE::REGISTRATION);
			break;
		case ID_L_MAIN:
			loginWindow->windowManager->ShowWindow(WINDOW_TYPE::MAIN);
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


void LoginWindow::InnerControlsProc(LPARAM lParam, WORD controlMsg)
{
	HWND hControl = (HWND)lParam;
	if (hControl == hLoginBtn)
	{
		if (AllFieldsFilled())
		{
			char* err_msg = nullptr;
			client->TryLogin(login, pass, err_msg);
		}
		else
		{
			MessageBox(hWnd, L"You should fill all fields!", L"NiceChat", MB_OK);
		}
	}
}


bool LoginWindow::AllFieldsFilled()
{
	GetWindowText(hLoginEdit, (TCHAR*)login, strBuffSize);
	GetWindowText(hPassEdit, (TCHAR*)pass, strBuffSize);
	if (strlen(login) == 0
		|| strlen(pass) == 0)
	{
		return false;
	}
	return true;
}