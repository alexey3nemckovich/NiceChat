#pragma once
#include "Window.h"


class MainWindow : 
	public Window
{
private:
	//inner controls
	//methods
	void Init();
	//wnd proc
	friend LRESULT CALLBACK MainWndProc(
		HWND hDlg,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	);
public:
	MainWindow();
	~MainWindow();
};