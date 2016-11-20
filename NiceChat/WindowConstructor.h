#pragma once
#include "WindowManager.h"


class WindowConstructor
{
private:
	WindowConstructor();
	~WindowConstructor();
public:
	static WindowConstructor* GetInstance();
	HWND CreateControl(LPCTSTR className, LPCTSTR text, HWND parentHWnd, int x, int y, int width, int height, DWORD style = 0);
};