#pragma once
#include "Window.h"


class LoginWindow :
	public Window
{
private:
	//inner controls
	HWND hPassText;
	HWND hPassEdit;
	HWND hLoginText;
	HWND hLoginEdit;
	HWND hLoginBtn;
	//fields
	const int strBuffSize = 50;
	char *login;
	char *pass;
	//methods
	void Init();
	bool AllFieldsFilled();
	void BtnClick(LPARAM lParam);
	//window proc
	friend LRESULT CALLBACK LoginProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	LoginWindow();
	~LoginWindow();
};