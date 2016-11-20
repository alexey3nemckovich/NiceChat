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
	//methods
	void Init();
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