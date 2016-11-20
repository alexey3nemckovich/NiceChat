#include "stdafx.h"
#include "RegistrationWindow.h"


RegistrationWindow::RegistrationWindow() 
	: Window(RegistrationProc, _T("RegWindowClass"), _T("Registration"), 400, 600)
{
	SetMenu(hWnd, LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(ID_REGISTRATION_MENU)));
	Init();
}


void RegistrationWindow::Init()
{

	POINT wndCenter = Window::GetHWNDCenter(hWnd);
	int w = 125;
	int h = 50;
	int loginBtnWidth = 200;
	int loginBtnHeight = 50;
	DWORD staticStyle = SS_CENTER | WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD editStyle = ES_CENTER | WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD editPassStyle = ES_PASSWORD | editStyle;
	DWORD btnStyle;
	//name
	hNameText = windowConstructor->CreateControl(L"STATIC", L"Name", hWnd, 50, 100, w, h, staticStyle);
	hNameEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 225, 100, w, h, editStyle);
	//last name
	hLastNameText = windowConstructor->CreateControl(L"STATIC", L"Login", hWnd, 50, 175, w, h, staticStyle);
	hLastNameEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 225, 175, w, h, editStyle);
	//login
	hLoginText = windowConstructor->CreateControl(L"STATIC", L"Login", hWnd, 50, 250, w, h, staticStyle);
	hLoginEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 225, 250, w, h, editStyle);
	//pass
	hPassText = windowConstructor->CreateControl(L"STATIC", L"Password", hWnd, 50, 325, w, h, staticStyle);
	hPassEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 225, 325, w, h, editPassStyle);
	//pass confirm
	hPassConfirmText = windowConstructor->CreateControl(L"STATIC", L"Password", hWnd, 50, 400, w, h, staticStyle);
	hPassConfirmEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 225, 400, w, h, editPassStyle);
	//reg btn
	hRegBtn = windowConstructor->CreateControl(L"BUTTON", L"Registrate", hWnd, wndCenter.x - loginBtnWidth / 2, 475, loginBtnWidth, loginBtnHeight);
	//INIT FIELDS
	name = (char*)malloc(strBuffSize);
	lastName = (char*)malloc(strBuffSize);
	login = (char*)malloc(strBuffSize);
	pass = (char*)malloc(strBuffSize);
	passConfirm = (char*)malloc(strBuffSize);
}


RegistrationWindow::~RegistrationWindow()
{

}


LRESULT CALLBACK RegistrationProc(
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
		static RegistrationWindow* regWindow = (RegistrationWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::REGISTRATION);
		// Parse the menu selections:
		switch (wmId)
		{
		case 0:
			regWindow->BtnClick(lParam);
			break;
		case ID_R_ABOUT:
			regWindow->dialogManager->ShowDialog(DIALOG_TYPE::ABOUT);
			break;
		case ID_R_LOGIN:
			regWindow->windowManager->ShowWindow(WINDOW_TYPE::LOGIN);
			break;
		case ID_R_MAIN:
			regWindow->windowManager->ShowWindow(WINDOW_TYPE::MAIN);
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


void RegistrationWindow::BtnClick(LPARAM lParam)
{
	HWND hBtn = (HWND)lParam;
	if (hBtn == hRegBtn)
	{
		if (AllFieldsFilled())
		{
			if (PasswordsMatch())
			{
				string err_msg;
				client->TryRegistrate(string(name), string(lastName), string(login), string(pass), &err_msg);
			}
			else
			{
				MessageBox(hWnd, L"Passwords don't match!", L"NiceChat", MB_OK);
			}
		}
		else
		{
			MessageBox(hWnd, L"You should fill all fields!", L"NiceChat", MB_OK);
		}
	}
}


bool RegistrationWindow::AllFieldsFilled()
{
	GetWindowText(hNameEdit, (TCHAR*)name, strBuffSize);
	GetWindowText(hLastNameEdit, (TCHAR*)lastName, strBuffSize);
	GetWindowText(hLoginEdit, (TCHAR*)login, strBuffSize);
	GetWindowText(hPassEdit, (TCHAR*)pass, strBuffSize);
	GetWindowText(hPassConfirmEdit, (TCHAR*)passConfirm, strBuffSize);
	int x = strlen(name);
	if (strlen(name) == 0
		|| strlen(lastName) == 0
		|| strlen(login) == 0
		|| strlen(pass) == 0
		|| strlen(passConfirm) == 0)
	{
		return false;
	}
	return true;
}


bool RegistrationWindow::PasswordsMatch()
{
	if (strcmp(pass, passConfirm) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}