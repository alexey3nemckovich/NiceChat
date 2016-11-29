#pragma once
#include "Window.h"
#include "Camera.h"
#include "ImageProcesser.h"


class MainWindow : 
	public Window
{
private:
	//inner controls
	HWND hListCapsComboBox;
	HWND hWebCamBox;
	HWND hOnlineClientsListBox;
	HWND hCallButton;
	HMENU hMenu;
	//methods
	void Init();
	void SetWindowTitle(char *newWndTitle);
	void SetOnlineClientsList(vector<ClientInfo> onlineClients);
	void InnerControlsProc(LPARAM, WORD);
	void RefreshCapDeviceToComboBox();
	void AddCapDeviceIndexToComboBox(int);
	void AddCapDeviceToComboBox(CaptureDevice);
	void RefreshControlsState();
	//friend procs
	friend LRESULT CALLBACK MainWndProc(
		HWND,
		UINT,
		WPARAM,
		LPARAM
	);
	//fields
	bool isAlive;
	const int webCamBoxLeft = 200;
	const int webCamBoxTop = 30;
	const int webCamBoxWidth = 640;
	const int webCamBoxHeight = 480;
	const RECT webCamBoxRect = RECT{webCamBoxLeft, webCamBoxTop, webCamBoxLeft + webCamBoxWidth, webCamBoxTop + webCamBoxHeight};
	Camera* camera;
	const ImageProcesser* imageProcesser;
	vector<CaptureDevice> listCaps;
	HANDLE webcamThread;
	bool webCamThreadSuspended;
	friend DWORD WINAPI CamRenderingProc(
		CONST LPVOID lpParam
	);
public:
	void Show();
	void Hide();
	void AddClientToListBox(char* clientLogin);
	void RemoveClientFromListBox(char* clientLogin);
	MainWindow();
	~MainWindow();
};