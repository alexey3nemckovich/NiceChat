#include "stdafx.h"
#include "WindowManager.h"
#include "Window.h"
#include "ChatWindowsList.h"


HINSTANCE WindowManager::hInstance;
int WindowManager::nCmdShow;


WindowManager::WindowManager()
{
	windows[WINDOW_TYPE::INCOMING_CALL] = new IncomingCallWindow();
	windows[WINDOW_TYPE::LOGIN] = new LoginWindow();
	windows[WINDOW_TYPE::MAIN] = new MainWindow();
	windows[WINDOW_TYPE::REGISTRATION] = new RegistrationWindow();
}


WindowManager::~WindowManager()
{
	for each(auto pair in windows)
	{
		delete(pair.second);
	}
}


WindowManager* WindowManager::GetInstance()
{
	static WindowManager instance = WindowManager();
	return &instance;
}


Window *WindowManager::GetWindow(WINDOW_TYPE wndType)
{
	return windows[wndType];
}


void WindowManager::ShowWindow(WINDOW_TYPE wndType, bool hide_active)
{
	Window* targetWindow = windows[wndType];
	if (hide_active)
	{
		if (activeWindow != NULL)
		{
			activeWindow->Hide();
		}
	}
	targetWindow->Show();
	activeWindow = (Window*)targetWindow;
}


Window* WindowManager::CreateWnd(WINDOW_TYPE wndType)
{
	switch (wndType)
	{
	case WINDOW_TYPE::MAIN:
		return new MainWindow();
	case WINDOW_TYPE::LOGIN:
		return new LoginWindow();
	case WINDOW_TYPE::REGISTRATION:
		return new RegistrationWindow();
	case WINDOW_TYPE::INCOMING_CALL:
		return new IncomingCallWindow();
	default:
		return NULL;
	}
}