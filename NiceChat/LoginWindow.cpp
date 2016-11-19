#include "stdafx.h"
#include "LoginWindow.h"


LoginWindow::LoginWindow() : Window(LOGIN_WINDOW, LoginWindow::LoginProc)
{

}


LoginWindow::~LoginWindow()
{

}


INT_PTR CALLBACK LoginWindow::LoginProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		//code
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return (INT_PTR)FALSE;
}