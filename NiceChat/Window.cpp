#include "stdafx.h"
#include "Window.h"


Client *Window::client = Client::GetInstance();
DialogManager *Window::dialogManager = DialogManager::GetInstance();
WindowManager *Window::windowManager = WindowManager::GetInstance();
WindowConstructor *Window::windowConstructor = WindowConstructor::GetInstance();
POINT Window::screenCenter = POINT{ 0, 0 };


Window::Window(WINDOW_PROC wnd_proc, LPCTSTR lpClassName, LPCTSTR lpWindowName, int width, int height, DWORD style)
{
	if (!style)
	{
		style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	}
	this->lpClassName = lpClassName;
	this->lpWindowName = lpWindowName;
	this->wndProc = wnd_proc;
	RegisterWindowClass();
	InitInstance(width, height, style);
}


ATOM Window::RegisterWindowClass()
{
	WNDCLASS wc;
	wc.hInstance = WindowManager::GetHInstance();
	wc.lpszClassName = lpClassName;
	wc.lpfnWndProc = this->wndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = NULL;
	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	return RegisterClass(&wc);
}


void Window::InitInstance(int width, int height, DWORD style)
{
	hWnd = CreateWindow
	(
		this->lpClassName,
		this->lpWindowName,
		//THICKFRAME makes window resizable
		style,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		HWND_DESKTOP,
		NULL,
		WindowManager::GetHInstance(),
		NULL
	);
}


Window::~Window()
{

}


void Window::Show()
{
	MoveToCenter(hWnd);
	ShowWindow(hWnd, WindowManager::GetNCmdShow());
}


void Window::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}


POINT Window::GetHWNDCenter(HWND hWnd)
{
	POINT center;
	RECT wndRect;
	GetWindowRect(hWnd, &wndRect);
	int width = wndRect.right - wndRect.left;
	int height = wndRect.bottom - wndRect.top;
	center.x = width / 2;
	center.y = height / 2;
	return center;
}


void Window::MoveToCenter(HWND hWnd)
{
	if (screenCenter.x == 0)
	{
		screenCenter = GetHWNDCenter(GetDesktopWindow());
	}
	RECT wndRect;
	GetWindowRect(hWnd, &wndRect);
	int width = wndRect.right - wndRect.left;
	int height = wndRect.bottom - wndRect.top;
	SetWindowPos(hWnd, NULL, screenCenter.x - width / 2, screenCenter.y - height / 2, width, height, 0);
}


void Window::SetText(HWND hWnd, char *newText)
{
	LPCWSTR lpNewText;
	lpNewText = PCharToLPCWSTR(newText);
	SetWindowText(hWnd, lpNewText);
	free((wchar_t*)lpNewText);
}


void Window::FillWithBrush(HWND hWnd, HBRUSH hBrush)
{
	HDC hDC = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	FillRect(hDC, &rect, hBrush);
	ReleaseDC(hWnd, hDC);
}