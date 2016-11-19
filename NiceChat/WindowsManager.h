#pragma once


enum class WINDOW_TYPE{MAIN, REGISTRATION, LOGIN, ABOUT};


class Window;
class AboutWindow;
class MainWindow;
class LoginWindow;
class RegistrationWindow;


class WindowsManager
{
private:
	AboutWindow *aboutWindow;
	MainWindow *mainWindow;
	LoginWindow *loginWindow;
	RegistrationWindow *regWindow;
	Window *GetTargetWindow(WINDOW_TYPE wndType);
	Window *activeWindow;
public:
	WindowsManager();
	~WindowsManager();
	static WindowsManager* GetInstance();
	void ShowWindow(WINDOW_TYPE wndType, bool hide_active = true);
};