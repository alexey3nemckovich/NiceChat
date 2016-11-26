#include "stdafx.h"
#include "WindowManager.h"
#include "Window.h"
#include "ChatWindowsList.h"


HINSTANCE WindowManager::hInstance;
int WindowManager::nCmdShow;


WindowManager::WindowManager()
{
	regWindow = NULL;
	loginWindow = NULL;
	mainWindow = NULL;
}


WindowManager::~WindowManager()
{
	delete(regWindow);
	delete(loginWindow);
	delete(mainWindow);
}


WindowManager* WindowManager::GetInstance()
{
	static WindowManager instance = WindowManager();
	return &instance;
}


void WindowManager::ShowWindow(WINDOW_TYPE wndType, bool hide_active)
{
	Window* targetWindow = GetWindow(wndType);
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


Window* WindowManager::GetWindow(WINDOW_TYPE wndType)
{
	switch (wndType)
	{
	case WINDOW_TYPE::MAIN:
		if (mainWindow == NULL)
		{
			mainWindow = new MainWindow();
		}
		return mainWindow;
	case WINDOW_TYPE::LOGIN:
		if (loginWindow == NULL)
		{
			loginWindow = new LoginWindow();
		}
		return loginWindow;
	case WINDOW_TYPE::REGISTRATION:
		if (regWindow == NULL)
		{
			regWindow = new RegistrationWindow();
		}
		return regWindow;
	default:
		return NULL;
	}
}