#include "stdafx.h"
#include "DialogProcedures.h"

INT_PTR CALLBACK AboutDialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (LOWORD(wParam) != ID_A_OK)
		{
			break;
		}
	}
	case WM_CLOSE:
	{
		ShowWindow(hwndDlg, SW_HIDE);
	}
	break;
	default:
		return false;
	}
	return true;
}