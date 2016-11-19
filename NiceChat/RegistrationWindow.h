#pragma once
#include "Window.h"


class RegistrationWindow :
	public Window
{
private:
	static INT_PTR CALLBACK RegistrationProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	RegistrationWindow();
	~RegistrationWindow();
};