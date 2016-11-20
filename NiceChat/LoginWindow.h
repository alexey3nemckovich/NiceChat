#pragma once
#include "Window.h"


class LoginWindow :
	public Window
{
private:
	void Init();
	static HWND hPassText;
	static HWND hPassEdit;
	static HWND hLoginText;
	static HWND hLoginEdit;
	static HWND hLoginBtn;
	static LRESULT CALLBACK LoginProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
	static void BtnClick(LPARAM lParam);
public:
	LoginWindow();
	~LoginWindow();
};