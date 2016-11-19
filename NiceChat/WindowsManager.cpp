#include "stdafx.h"
#include "WindowsManager.h"
#include "Window.h"
#include "ChatWindowsList.h"


WindowsManager::WindowsManager()
{
	aboutWindow = NULL;
	regWindow = NULL;
	loginWindow = NULL;
	mainWindow = NULL;
}


WindowsManager::~WindowsManager()
{
	delete(aboutWindow);
	delete(regWindow);
	delete(loginWindow);
	delete(mainWindow);
}


WindowsManager* WindowsManager::GetInstance()
{
	static WindowsManager* instance = new WindowsManager();
	return instance;
}


void WindowsManager::ShowWindow(WINDOW_TYPE wndType, bool hide_active)
{
	Window* targetWindow = GetTargetWindow(wndType);
	if (hide_active)
	{
		if (activeWindow != NULL)
		{
			activeWindow->Hide();
		}
	}
	targetWindow->Show();
	activeWindow = targetWindow;
}


Window* WindowsManager::GetTargetWindow(WINDOW_TYPE wndType)
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
	case WINDOW_TYPE::ABOUT:
		if (aboutWindow == NULL)
		{
			aboutWindow = new AboutWindow();
		}
		return aboutWindow;
	default:
		return NULL;
	}
}