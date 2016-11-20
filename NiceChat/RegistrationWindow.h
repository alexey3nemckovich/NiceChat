#pragma once
#include "Window.h"


class RegistrationWindow :
	public Window
{
private:
	static LRESULT CALLBACK RegistrationProc(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
	void Init();
public:
	RegistrationWindow();
	~RegistrationWindow();
};