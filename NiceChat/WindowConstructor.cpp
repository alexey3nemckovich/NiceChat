#include "stdafx.h"
#include "WindowConstructor.h"


WindowConstructor::WindowConstructor()
{

}


WindowConstructor::~WindowConstructor()
{

}


WindowConstructor* WindowConstructor::GetInstance()
{
	static WindowConstructor* windowsContructor = new WindowConstructor();
	return windowsContructor;
}


HWND WindowConstructor::CreateControl(LPCTSTR className, LPCTSTR text, HWND parentHWnd, int x, int y, int width, int height, DWORD style)
{
	DWORD stdStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD;
	DWORD dwStyle = style == 0 ? stdStyle : style;
	HWND hControl = CreateWindow(
		className,  // Predefined class; Unicode assumed 
		text,     // Button text 
		dwStyle,  // Styles 
		x,         // x position 
		y,         // y position 
		width,        // Button width
		height,        // Button height
		parentHWnd,     // Parent window
		NULL,       // No menu.
		WindowManager::GetHInstance(),
		NULL);      // Pointer not needed.
	return hControl;
}