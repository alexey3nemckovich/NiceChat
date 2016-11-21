#pragma once
#include "Window.h"


class RegistrationWindow :
	public Window
{
private:
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
	const int strBuffSize = 50;
	char *name;
	char *lastName;
	char *login;
	char *pass;
	char *passConfirm;
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
public:
	RegistrationWindow();
	~RegistrationWindow();
};