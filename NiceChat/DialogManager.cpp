#include "stdafx.h"
#include "DialogManager.h"
#include "Window.h"


DialogManager::DialogManager()
{
	aboutDialog = NULL;
}


DialogManager::~DialogManager()
{
	delete(aboutDialog);
}


DialogManager* DialogManager::GetInstance()
{
	static DialogManager dialogManager = DialogManager();
	return &dialogManager;
}


void DialogManager::ShowDialog(DIALOG_TYPE dlgType)
{
	HWND dlg = GetDialog(dlgType);
	Window::MoveToCenter(dlg);
	ShowWindow(dlg, WindowManager::GetNCmdShow());
}


HWND DialogManager::GetDialog(DIALOG_TYPE dlgType)
{
	switch (dlgType)
	{
	case DIALOG_TYPE::ABOUT:
		if (aboutDialog == NULL)
		{
			aboutDialog = CreateDialog(WindowManager::GetHInstance(), MAKEINTRESOURCE(ABOUT_WINDOW), NULL, AboutDialogProc);
		}
		return aboutDialog;
	default:
		return NULL;
	}
}


void DialogManager::ShowError(char *err_msg)
{
	LPCWSTR l_err_msg = PCharToLPCWSTR(err_msg);
	MessageBox(NULL, l_err_msg, L"Error", 0);
	free((void*)l_err_msg);
}