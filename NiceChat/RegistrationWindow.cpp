#include "stdafx.h"
#include "RegistrationWindow.h"


LPCWSTR PCharToLPCWSTR(char* str);


RegistrationWindow::RegistrationWindow() 
	: Window(RegistrationProc, _T("RegWindowClass"), _T("Registration"), 400, 600, 0)
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
	DWORD staticStyle = SS_CENTER | WS_BORDER | WS_VISIBLE | WS_CHILD;
	DWORD editStyle = ES_CENTER | WS_BORDER | WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD editPassStyle = ES_PASSWORD | editStyle;
	DWORD btnStyle = 0;
	//name
	hNameText = windowConstructor->CreateControl(L"STATIC", L"Name", hWnd, 50, 100, w, h, staticStyle);
	hNameEdit = windowConstructor->CreateControl(L"EDIT", L"", hWnd, 225, 100, w, h, editStyle);
	//last name
	hLastNameText = windowConstructor->CreateControl(L"STATIC", L"Last name", hWnd, 50, 175, w, h, staticStyle);
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
	hRegBtn = windowConstructor->CreateControl(L"BUTTON", L"Registrate", hWnd, wndCenter.x - loginBtnWidth / 2, 475, loginBtnWidth, loginBtnHeight, btnStyle);
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
			regWindow->InnerControlsProc(lParam, HIWORD(wParam));
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


void RegistrationWindow::InnerControlsProc(LPARAM lParam, WORD controlMsg)
{
	HWND hControl = (HWND)lParam;
	if (hControl == hRegBtn)
	{
		if (AllFieldsFilled())
		{
			if (PasswordsMatch())
			{
				char err_msg[100];
				if (!client->TryRegistrate(name, lastName, login, pass, err_msg))
				{
					dialogManager->ShowError(err_msg);
				}
				else
				{
					windowManager->ShowWindow(WINDOW_TYPE::MAIN);
				}
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
	TCHAR buff[STR_BUFF_SIZE];
	GetWindowText(hNameEdit, buff, STR_BUFF_SIZE);
	CharToOem(buff, name);
	GetWindowText(hLastNameEdit, buff, STR_BUFF_SIZE);
	CharToOem(buff, lastName);
	GetWindowText(hLoginEdit, buff, STR_BUFF_SIZE);
	CharToOem(buff, login);
	GetWindowText(hPassEdit, buff, STR_BUFF_SIZE);
	CharToOem(buff, pass);
	GetWindowText(hPassConfirmEdit, buff, STR_BUFF_SIZE);
	CharToOem(buff, passConfirm);
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