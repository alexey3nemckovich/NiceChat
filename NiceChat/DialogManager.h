#pragma once
#include "DialogProcedures.h"
#include "WindowManager.h"
#include <map>


typedef INT_PTR CALLBACK DialogProc(
	_In_ HWND   hwndDlg,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);


enum class DIALOG_TYPE {
	ABOUT
};


class DialogManager
{
private:
	DialogManager();
	~DialogManager();
	std::map<DIALOG_TYPE, HWND> dialogs;
	HWND DialogManager::CreateDialogWindow(DIALOG_TYPE dlgType);
public:
	static DialogManager* GetInstance();
	void ShowDialog(DIALOG_TYPE);
	void ShowError(char *msg);
};