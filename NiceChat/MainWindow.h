#pragma once
#include "Window.h"


class MainWindow : 
	public Window
{
private:
	static LRESULT CALLBACK MainWndProc(
		HWND hDlg,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
	void Init();
public:
	MainWindow();
	~MainWindow();
};