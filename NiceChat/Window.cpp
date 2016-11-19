#include "stdafx.h"
#include "Window.h"


WindowsManager *Window::windowsManager;


Window::Window(int resource_ID, DIALOG_PROC wnd_proc)
{
	hWnd = CreateDialog(NULL, MAKEINTRESOURCE(resource_ID), NULL, wnd_proc);
	windowsManager = WindowsManager::GetInstance();
}


Window::~Window()
{

}


void Window::Show()
{
	ShowWindow(hWnd, SW_SHOW);
}


void Window::Hide()
{
	ShowWindow(hWnd, SW_HIDE);
}