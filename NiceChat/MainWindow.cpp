#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() 
	: Window(MainWndProc, _T("MainWindowClass"), _T("NiceChat"), 900, 600)
{
	SetMenu(hWnd, LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(IDC_NICECHAT)));
	Init();
}


MainWindow::~MainWindow()
{
	if (webcamThread != NULL)
	{
		isAlive = false;
		if (webCamThreadSuspended)
		{
			ResumeThread(webcamThread);
		}
		WaitForSingleObject(webcamThread, INFINITE);
		CloseHandle(webcamThread);
		webcamThread = NULL;
		delete(camera);
	}
}


void MainWindow::Init()
{
	isAlive = true;
	imageProcesser = ImageProcesser::GetInstance();
	webcamThread = CreateThread(NULL, 0, &(CamRenderingProc), NULL, CREATE_SUSPENDED, 0);
	webCamThreadSuspended = true;
	DWORD cmbBoxStyle = CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP;
	hListCapsComboBox = windowConstructor->CreateControl(L"COMBOBOX", L"", hWnd, webCamBoxLeft, 0, 40, 100, cmbBoxStyle);
	DWORD camBoxStyle = SS_CENTER | WS_BORDER | WS_VISIBLE | WS_CHILD | SS_BITMAP;
	hWebCamBox = windowConstructor->CreateControl(
		L"STATIC",
		L"",
		hWnd,
		webCamBoxLeft,
		webCamBoxTop,
		webCamBoxWidth,
		webCamBoxHeight,
		camBoxStyle);
	camera = new Camera(0, webCamBoxWidth, webCamBoxHeight, hWnd);
}


void MainWindow::RefreshCapDeviceToComboBox()
{
	SendMessage(hListCapsComboBox, CB_RESETCONTENT, 0, 0);
	//listCaps = Camera::GetListCaps();
	int countListCaps = Camera::GetCamsCount();
	for (int i = 0; i < countListCaps; i++)
	{
		//AddCapDeviceToComboBox(listCaps[i]);
		AddCapDeviceIndexToComboBox(i);
	}
}


void MainWindow::AddCapDeviceIndexToComboBox(int capIndex)
{
	TCHAR capIndexStr[10];
	_stprintf_s(capIndexStr, L"%i", capIndex);
	SendMessage(hListCapsComboBox, CB_ADDSTRING, 0, (LPARAM)capIndexStr);
}


void MainWindow::AddCapDeviceToComboBox(CaptureDevice capDevice)
{
	TCHAR fullCapDescript[capDevice.cbName + capDevice.cbVer];
	_stprintf_s(fullCapDescript, L"%s %s", capDevice.lpszName, capDevice.lpszDescription);
	SendMessage(hListCapsComboBox, CB_ADDSTRING, 0, (LPARAM)fullCapDescript);
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	static MainWindow* mainWindow;
	switch (message)
	{
	case WM_COMMAND:
	{
		mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);
		int wmId = LOWORD(wParam);	
		// Parse the menu selections:
		switch (wmId)
		{
		case 0:
			mainWindow->InnerControlsProc(lParam, HIWORD(wParam));
			break;
		case ID_M_ABOUT:
			mainWindow->dialogManager->ShowDialog(DIALOG_TYPE::ABOUT);
			break;
		case ID_M_LOGIN:
			mainWindow->windowManager->ShowWindow(WINDOW_TYPE::LOGIN);
			break;
		case ID_M_REGISTRATE:
			mainWindow->windowManager->ShowWindow(WINDOW_TYPE::REGISTRATION);
			break;
		case ID_M_LEAVE_CHAT:
			break;
		case ID_M_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


void MainWindow::InnerControlsProc(LPARAM lParam, WORD controlMsg)
{
	HWND hControl = (HWND)lParam;
	if (hControl == hListCapsComboBox)
	{
		switch (controlMsg)
		{
		case CBN_SELCHANGE:
			int capIndex;
			capIndex = SendMessage(hListCapsComboBox, CB_GETCURSEL, 0, 0);
			if (capIndex != camera->GetCapDeviceNumber())
			{
				camera->SetCapDeviceIndex(capIndex);
			}
			break;
		default:
			break;
		}
	}
}


void MainWindow::Show()
{
	if (client->IsOnline())
	{
		char newWndTitle[1000];
		sprintf(newWndTitle, "NiceChat - %s %s", client->Name(), client->LastName());
		SetWinowTitle(newWndTitle);
		vector<ClientInfo> onlineClientsList = client->GetOnlineClientsList();
		SetOnlineClientsLits(onlineClientsList);
	}
	RefreshCapDeviceToComboBox();
	Window::Show();
	camera->Open();
	ResumeThread(webcamThread);
	webCamThreadSuspended = false;
}


void MainWindow::SetWinowTitle(char *newWndTitle)
{
	LPCWSTR lpNewWndTitle;
	lpNewWndTitle = PCharToLPCWSTR(newWndTitle);
	SetWindowText(hWnd, lpNewWndTitle);
	free((wchar_t*)lpNewWndTitle);
}


void MainWindow::SetOnlineClientsLits(vector<ClientInfo> onlineClients)
{

}


void MainWindow::Hide()
{
	SuspendThread(webcamThread);
	webCamThreadSuspended = true;
	camera->Close();
	Window::Hide();
}


DWORD WINAPI CamRenderingProc(CONST LPVOID lParam)
{
	static MainWindow* mainWnd = (MainWindow*)WindowManager::GetInstance()
		->GetWindow(WINDOW_TYPE::MAIN);
	static HWND hCamBox = mainWnd->hWebCamBox;
	static Camera* cam = mainWnd->camera;
	static const ImageProcesser* imageProcesser = mainWnd->imageProcesser;
	static HDC hCamBoxDC;
	static HDC hBuffDC = CreateCompatibleDC(NULL);
	static cv::Mat camFrame;
	static const int webCamBoxWidth = mainWnd->webCamBoxWidth;
	static const int webCamBoxHeight = mainWnd->webCamBoxHeight;
	while (mainWnd->isAlive)
	{
		camFrame = cam->GetFrame();
		HBITMAP cross = imageProcesser->ConvertCVMatToHBITMAP(camFrame);
		SelectObject(hBuffDC, cross);
		hCamBoxDC = GetDC(hCamBox);
		BitBlt(hCamBoxDC, 0, 0, webCamBoxWidth, webCamBoxHeight, hBuffDC, 0, 0, SRCCOPY);
		ReleaseDC(hCamBox, hCamBoxDC);
	}
	return 0;
}