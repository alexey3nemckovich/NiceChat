#include "stdafx.h"
#include "Window.h"


DialogManager *Window::dialogManager = DialogManager::GetInstance();
WindowManager *Window::windowsManager = WindowManager::GetInstance();
WindowConstructor *Window::windowConstructor = WindowConstructor::GetInstance();
POINT Window::screenCenter = POINT{ 0, 0 };


Window::Window(WINDOW_PROC wnd_proc, LPCTSTR lpClassName, LPCTSTR lpWindowName, int width, int height)
{
	this->lpClassName = lpClassName;
	this->lpWindowName = lpWindowName;
	this->wndProc = wnd_proc;
	RegisterWindowClass();
	InitInstance(width, height);
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
	wc.hbrBackground = NULL;
	return RegisterClass(&wc);
}


void Window::InitInstance(int width, int height)
{
	hWnd = CreateWindow
	(
		this->lpClassName,
		this->lpWindowName,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
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


void Window::Hide() const
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