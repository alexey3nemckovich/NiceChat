#pragma once
#include "Window.h"


class MainWindow : 
	public Window
{
private:
	static INT_PTR CALLBACK MainWndProc(
		HWND hDlg,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	MainWindow();
	~MainWindow();
};