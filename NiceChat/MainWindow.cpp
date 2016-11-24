#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() 
	: Window(MainWndProc, _T("MainWindowClass"), _T("NiceChat"), 800, 600)
{
	SetMenu(hWnd, LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(IDC_NICECHAT)));
	Init();
}


void MainWindow::Init()
{
	selectedCapIndex = -1;
	camera = NULL;
	webcamThread = CreateThread(NULL, 0, &(CamRenderingProc), NULL, CREATE_SUSPENDED, 0);
	DWORD cmbBoxStyle = CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP;
	hListCapsComboBox = windowConstructor->CreateControl(L"COMBOBOX", L"", hWnd, 100, 100, 400, 100, cmbBoxStyle);
	RefreshCapDeviceToComboBox();
	camera = new Camera(0, hWnd);
	camera->Open();
}


void MainWindow::RefreshCapDeviceToComboBox()
{
	SendMessage(hListCapsComboBox, CB_RESETCONTENT, 0, 0);
	//listCaps = Camera::GetListCaps();
	//int countListCaps = Camera::GetCamsCount();
	//for (int i = 0; i < countListCaps; i++)
	//{
	//	//AddCapDeviceToComboBox(listCaps[i]);
	//	AddCapDeviceIndexToComboBox(i);
	//}
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


MainWindow::~MainWindow()
{
	CloseHandle(webcamThread);
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		static MainWindow* mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);
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
			if (selectedCapIndex == -1 || capIndex != selectedCapIndex)
			{
				selectedCapIndex = capIndex;
				if (camera != NULL)
				{
					delete(camera);
				}
				camera = new Camera(capIndex, hWnd);
			}
			break;
		default:
			break;
		}
	}
}


void MainWindow::Show()
{
	RefreshCapDeviceToComboBox();
	Window::Show();
	ResumeThread(webcamThread);
}


void MainWindow::Hide()
{
	SuspendThread(webcamThread);
	Window::Hide();
}


DWORD WINAPI CamRenderingProc(CONST LPVOID lParam)
{
	static MainWindow* mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);
	static HWND hWnd = mainWindow->hWnd;
	static HDC hWdc;
	static HDC hdc = CreateCompatibleDC(NULL);
	static cv::Mat frame;
	while (1)
	{
		frame = mainWindow->camera->GetFrame();
		cv::imwrite("temp.bmp", frame);
		HBITMAP cross = (HBITMAP)LoadImage(NULL, L"temp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		SelectObject(hdc, cross);
		hWdc = GetDC(hWnd);
		BitBlt(hWdc, 0, 0, 500, 500, hdc, 0, 0, SRCCOPY);
		ReleaseDC(hWnd, hWdc);
		cv::waitKey(1000);
	}
}