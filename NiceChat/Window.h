#pragma once
#include "WindowManager.h"
#include "WindowConstructor.h"
#include <CommCtrl.h>
#include "resource.h"
#include <Windows.h>
#include "DialogManager.h"
#include "Client.h"


typedef LRESULT(CALLBACK *WINDOW_PROC)(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
);


class Window
{
private:
	LPCTSTR lpClassName;
	LPCTSTR lpWindowName;
	WINDOW_PROC wndProc;
	ATOM RegisterWindowClass();
	void InitInstance(int width, int height, DWORD style);
protected:
	Window(WINDOW_PROC wnd_proc, LPCTSTR lpClassName, LPCTSTR lpWindowName, int width, int height, DWORD style);
	~Window();
	friend class WindowManager;
	static Client *client;
	static DialogManager *dialogManager;
	static WindowManager *windowManager;
	static WindowConstructor* windowConstructor;
	static POINT screenCenter;
	static POINT GetHWNDCenter(HWND hWnd);
	HWND hWnd;
	virtual void Init() = 0;
	virtual void InnerControlsProc(LPARAM, WORD) = 0;
public:
	HWND GetHWnd() const
	{
		return hWnd;
	}
	virtual void Show();
	virtual void Hide();
	static void MoveToCenter(HWND hWnd);
	static void SetText(HWND, char*);
	static void DrawTextInCenter(HWND hWnd);
};