#include "stdafx.h"
#include "IncomingCallWindow.h"
#include "MainWindow.h"


IncomingCallWindow::IncomingCallWindow()
	: Window(IncomingCallWndProc, _T("IncomingCallWindowClass"), _T("Call"), 300, 150, WS_OVERLAPPED | WS_CAPTION)
{
	Init();
}


void IncomingCallWindow::Init()
{
	POINT wndCenter = Window::GetHWNDCenter(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	DWORD staticStyle = SS_CENTER | WS_BORDER | WS_VISIBLE | WS_CHILD;
	DWORD btnStyle = 0;
	int statWidth = 200;
	int statHeight = 50;
	hCallStat = windowConstructor->CreateControl(L"STATIC", L"Call from ", hWnd, (rect.right - statWidth)/2, 20, statWidth, statHeight, staticStyle);
	//btns
	int btnWidth = 100;
	int btnHeight = 30;
	hAcceptBtn = windowConstructor->CreateControl(L"BUTTON", L"Accept", hWnd, 0, rect.bottom - btnHeight, btnWidth, btnHeight, btnStyle);
	hCancelBtn = windowConstructor->CreateControl(L"BUTTON", L"Cancel", hWnd, rect.right - btnWidth, rect.bottom - btnHeight, btnWidth, btnHeight, btnStyle);
}


IncomingCallWindow::~IncomingCallWindow()
{
}


LRESULT CALLBACK IncomingCallWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	static IncomingCallWindow* incomingCallWindow;
	static Client* client = Client::GetInstance();
	switch (message)
	{
	case WM_COMMAND:
	{
		incomingCallWindow = (IncomingCallWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::INCOMING_CALL);
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case 0:
			incomingCallWindow->InnerControlsProc(lParam, HIWORD(wParam));
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		//code	
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
	{
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void IncomingCallWindow::InnerControlsProc(LPARAM lParam, WORD controlMsg)
{
	static MainWindow* mainWindow = (MainWindow*)windowManager->GetWindow(WINDOW_TYPE::MAIN);
	HWND hControl = (HWND)lParam;
	if (hControl == hAcceptBtn)
	{
		client->AcceptIncomingCall();
	}
	if (hControl == hCancelBtn)
	{
		client->CancelIncomingCall();
	}
	mainWindow->RefreshControls(client->GetStatus());
	Hide();
}


void IncomingCallWindow::SetCallerLogin(char *callerLogin)
{
	char callStr[STR_BUFF_SIZE];
	sprintf(callStr, "Call from '%s'", callerLogin);
	LPCWSTR lpLogin = PCharToLPCWSTR(callStr);
	SetWindowText(hCallStat, lpLogin);
	free((void*)lpLogin);
}