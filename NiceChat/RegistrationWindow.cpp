#include "stdafx.h"
#include "RegistrationWindow.h"


RegistrationWindow::RegistrationWindow() : Window(REGISTRATION_WINDOW, RegistrationWindow::RegistrationProc)
{

}


RegistrationWindow::~RegistrationWindow()
{

}


INT_PTR CALLBACK RegistrationWindow::RegistrationProc(
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