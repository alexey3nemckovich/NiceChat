#pragma once
#include "DialogProcedures.h"
#include "WindowManager.h"


enum class DIALOG_TYPE {
	ABOUT
};


class DialogManager
{
private:
	DialogManager();
	~DialogManager();
	HWND aboutDialog;
	HWND GetDialog(DIALOG_TYPE dlgType);
public:
	static DialogManager* GetInstance();
	void ShowDialog(DIALOG_TYPE);
};