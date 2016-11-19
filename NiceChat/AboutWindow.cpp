#include "stdafx.h"
#include "AboutWindow.h"


AboutWindow::AboutWindow() : Window(ABOUT_WINDOW, AboutWindow::AboutProc)
{

}


AboutWindow::~AboutWindow()
{

}


INT_PTR CALLBACK AboutWindow::AboutProc(
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
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}