#pragma once
#include "Window.h"


class LoginWindow :
	public Window
{
private:
	static INT_PTR CALLBACK LoginProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	LoginWindow();
	~LoginWindow();
};