#pragma once
#include <map>


enum class WINDOW_TYPE{MAIN, REGISTRATION, LOGIN, ABOUT, INCOMING_CALL};


class Window;
class MainWindow;
class LoginWindow;
class RegistrationWindow;
class IncomingCallWindow;


class WindowManager
{
private:
	static HINSTANCE hInstance;
	static int nCmdShow;
	std::map<WINDOW_TYPE, Window*> windows;
	Window *activeWindow;
	Window *CreateWnd(WINDOW_TYPE wndType);
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
	Window *GetWindow(WINDOW_TYPE);
	void ShowWindow(WINDOW_TYPE wndType, bool hide_active = true);
};