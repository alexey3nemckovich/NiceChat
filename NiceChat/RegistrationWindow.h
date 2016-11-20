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
	//methods
	void Init();
	bool PasswordsMatch();
	void BtnClick(LPARAM lParam);
	//wnd proc
	friend LRESULT CALLBACK RegistrationProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	RegistrationWindow();
	~RegistrationWindow();
};