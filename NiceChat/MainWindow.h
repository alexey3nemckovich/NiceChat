#pragma once
#include "Window.h"
#include "IncomingCallWindow.h"
#include "Camera.h"
#include "ImageProcesser.h"


class MainWindow : 
	public Window
{
private:
	MainWindow();
	~MainWindow();
	friend class WindowManager;
	//inner controls
	HWND hListCapsComboBox;
	HWND hWebCamBox;
	HWND hOnlineClientsListBox;
	HWND hCallButton;
	HMENU hMenu;
	//methods
	void Init();
	void RefreshOnlineClientsList();
	void InnerControlsProc(LPARAM, WORD);
	void AddCapDeviceIndexToComboBox(int);
	void AddCapDeviceToComboBox(CaptureDevice);
	void RefreshCameraComponents();
	int GetListBoxSelectedClient(char* selectedClient);
	//wnd proc
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
	HANDLE hRenderWebcamThread;
	HANDLE hCallThread;
	bool webCamRenderThreadSuspended;
	//Thread proc
	friend DWORD WINAPI CamRenderThreadProc(
		CONST LPVOID lParam
	);
	friend DWORD WINAPI CallThreadProc(
		CONST LPVOID lParam
	);
public:
	void Show();
	void Hide();
	void AddClientToListBox(char* clientLogin);
	void RemoveClientFromListBox(char* clientLogin);
	void RenderMatFrame(cv::Mat frame);
	void RenderFrame(const uchar* frameData);
	void CallClick(char *clientLogin);
	void CancelCallClick();
	void FinishChattingClick();
	void ShowIncomingCall(char *callerLogin);
	void RefreshControls(ClientStatus clientStatus, char *clientLogin = "");
};