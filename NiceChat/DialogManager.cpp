#include "stdafx.h"
#include "DialogManager.h"
#include "Window.h"


DialogManager::DialogManager()
{
	for each(auto pair in dialogs)
	{
		pair.second = NULL;
	}
}


DialogManager::~DialogManager()
{

}


DialogManager* DialogManager::GetInstance()
{
	static DialogManager dialogManager = DialogManager();
	return &dialogManager;
}


void DialogManager::ShowDialog(DIALOG_TYPE dlgType)
{
	if (dialogs[dlgType] == NULL)
	{
		dialogs[dlgType] = CreateDialogWindow(dlgType);
	}
	HWND hDlg = dialogs[dlgType];
	Window::MoveToCenter(hDlg);
	ShowWindow(hDlg, WindowManager::GetNCmdShow());
}


HWND DialogManager::CreateDialogWindow(DIALOG_TYPE dlgType)
{
	switch (dlgType)
	{
	case DIALOG_TYPE::ABOUT:
		return CreateDialog(
			WindowManager::GetHInstance(),
			MAKEINTRESOURCE(ABOUT_WINDOW),
			NULL,
			AboutDialogProc
		);
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