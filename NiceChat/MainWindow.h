#pragma once

#include "resource.h"
#define MAX_LOADSTRING 100

class MainWindow
{
private:
	static HINSTANCE hInst;
	ATOM RegisterClass(HINSTANCE hInstance);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
public:
	MainWindow(HINSTANCE hInstance, int nCmdShow);
	~MainWindow();
};