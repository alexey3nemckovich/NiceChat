#pragma once


enum class WINDOW_TYPE{MAIN, REGISTRATION, LOGIN, ABOUT};


class Window;
class AboutWindow;
class MainWindow;
class LoginWindow;
class RegistrationWindow;


class WindowManager
{
private:
	static HINSTANCE hInstance;
	static int nCmdShow;
	AboutWindow *aboutWindow;
	MainWindow *mainWindow;
	LoginWindow *loginWindow;
	RegistrationWindow *regWindow;
	Window *activeWindow;
public:
	WindowManager();
	~WindowManager();
	static void SetAppParams(HINSTANCE hInstanceVal, int nCmdShowVal)
	{
		hInstance = hInstanceVal;
		nCmdShow = nCmdShowVal;
	}
	static HINSTANCE GetHInstance()
	{
		return hInstance;
	}
	static int GetNCmdShow()
	{
		return nCmdShow;
	}
	static WindowManager* GetInstance();
	Window *GetActiveWindow()
	{
		return activeWindow;
	}
	Window *GetWindow(WINDOW_TYPE wndType);
	void ShowWindow(WINDOW_TYPE wndType, bool hide_active = true);
};