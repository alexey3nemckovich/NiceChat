#pragma once
#include "WindowsManager.h"
#include "resource.h"
#include <Windows.h>


typedef INT_PTR(CALLBACK *DIALOG_PROC)(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);


class Window
{
protected:
	HWND hWnd;
	static WindowsManager *windowsManager;
public:
	Window(int resource_ID, DIALOG_PROC wnd_proc);
	~Window();
	void Show();
	void Hide();
};