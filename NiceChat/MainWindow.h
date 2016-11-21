#pragma once
#include "Window.h"
#include "Camera.h"


class MainWindow : 
	public Window
{
private:
	//inner controls
	HWND hCapsBtn;
	HWND hListCapsComboBox;
	//methods
	void Init();
	void InnerControlsProc(LPARAM, WORD);
	void RefreshCapDeviceToComboBox();
	void AddCapDeviceToComboBox(CaptureDevice);
	//wnd proc
	friend LRESULT CALLBACK MainWndProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM
	);
	//fields
	Camera *camera;
	int selectedCapIndex;
	vector<CaptureDevice> listCaps;
public:
	MainWindow();
	~MainWindow();
};