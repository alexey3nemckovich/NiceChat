#pragma once
#include "Window.h"


class LoginWindow :
	public Window
{
private:
	LoginWindow();
	~LoginWindow();
	friend class WindowManager;
	//inner controls
	HWND hPassText;
	HWND hPassEdit;
	HWND hLoginText;
	HWND hLoginEdit;
	HWND hLoginBtn;
	//fields
	char *login;
	char *pass;
	//methods
	void Init();
	bool AllFieldsFilled();
	void InnerControlsProc(LPARAM, WORD);
	//window proc
	friend LRESULT CALLBACK LoginProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM
	);
};