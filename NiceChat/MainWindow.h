#pragma once
#include "Window.h"
#include "Camera.h"


class MainWindow : 
	public Window
{
private:
	//inner controls
	HWND hListCapsComboBox;
	//methods
	void Init();
	void InnerControlsProc(LPARAM, WORD);
	void RefreshCapDeviceToComboBox();
	void AddCapDeviceIndexToComboBox(int);
	void AddCapDeviceToComboBox(CaptureDevice);
	//friend procs
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
	HANDLE webcamThread;
	friend DWORD WINAPI CamRenderingProc(
		CONST LPVOID lpParam
	);
public:
	void Show();
	void Hide();
	MainWindow();
	~MainWindow();
};