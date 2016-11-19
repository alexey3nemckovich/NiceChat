#pragma once
#include "Window.h"


class AboutWindow :
	public Window
{
private:
	static INT_PTR CALLBACK AboutProc(
		HWND hDlg,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	AboutWindow();
	~AboutWindow();
};