#pragma once
#include "Window.h"


class RegistrationWindow :
	public Window
{
private:
	RegistrationWindow();
	~RegistrationWindow();
	friend class WindowManager;
	//inner controls
	HWND hNameText;
	HWND hNameEdit;
	HWND hLastNameText;
	HWND hLastNameEdit;
	HWND hLoginText;
	HWND hLoginEdit;
	HWND hPassText;
	HWND hPassEdit;
	HWND hPassConfirmText;
	HWND hPassConfirmEdit;
	HWND hRegBtn;
	//fields
	char name[STR_BUFF_SIZE];
	char lastName[STR_BUFF_SIZE];
	char login[STR_BUFF_SIZE];
	char pass[STR_BUFF_SIZE];
	char passConfirm[STR_BUFF_SIZE];
	//methods
	void Init();
	bool PasswordsMatch();
	bool AllFieldsFilled();
	void InnerControlsProc(LPARAM, WORD);
	//wnd proc
	friend LRESULT CALLBACK RegistrationProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM
	);
};