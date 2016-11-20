#pragma once
#include "Window.h"


class LoginWindow :
	public Window
{
private:
	void Init();
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