#pragma once
#include "Window.h"


class IncomingCallWindow
	: public Window
{
private:
	IncomingCallWindow();
	~IncomingCallWindow();
	friend class WindowManager;
	//inner controls
	HWND hCallStat;
	HWND hAcceptBtn;
	HWND hCancelBtn;
	//methods
	void Init();
	void InnerControlsProc(LPARAM lParam, WORD controlMsg);
	//wnd proc
	friend LRESULT CALLBACK IncomingCallWndProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM
	);
public:
	void SetCallerLogin(char *callerLogin);
};