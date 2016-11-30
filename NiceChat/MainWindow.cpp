#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() 
	: Window(MainWndProc, _T("MainWindowClass"), _T("NiceChat"), 900, 600)
{
	hMenu = LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(IDC_NICECHAT));
	SetMenu(hWnd, hMenu);
	Init();
}


MainWindow::~MainWindow()
{
	if (hWebcamThread != NULL)
	{
		isAlive = false;
		if (webCamThreadSuspended)
		{
			ResumeThread(hWebcamThread);
		}
		WaitForSingleObject(hWebcamThread, INFINITE);
		CloseHandle(hWebcamThread);
		hWebcamThread = NULL;
		delete(camera);
	}
}


void MainWindow::Init()
{
	isAlive = true;
	imageProcesser = ImageProcesser::GetInstance();
	//webcamThread = CreateThread(NULL, 0, &(CamRenderingProc), NULL, CREATE_SUSPENDED, 0);
	//webCamThreadSuspended = true;
	//init conrtols
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
		camBoxStyle
	);
	int callBtnHeight = 60;
	int clientsListBoxLeft = 40;
	int clientsListBoxWidth = 200 - 10 - clientsListBoxLeft;
	int clientsListBoxHeight = webCamBoxHeight + 10 - callBtnHeight - 20;
	DWORD clientsListBoxStyle = LBS_HASSTRINGS | LBS_STANDARD | WS_VISIBLE | WS_CHILD;
	hOnlineClientsListBox = windowConstructor->CreateControl(
		L"LISTBOX",
		L"",
		hWnd,
		clientsListBoxLeft,
		webCamBoxTop,
		clientsListBoxWidth,
		clientsListBoxHeight,
		clientsListBoxStyle
	);
	DWORD callButtonStyle;
	hCallButton = windowConstructor->CreateControl(
		L"BUTTON",
		L"Call",
		hWnd,
		clientsListBoxLeft,
		webCamBoxTop + clientsListBoxHeight + 10,
		clientsListBoxWidth,
		callBtnHeight
	);
	//init camera
	//camera = new Camera(0, webCamBoxWidth, webCamBoxHeight, hWnd);
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


void MainWindow::AddClientToListBox(char* clientLogin)
{
	LPCWSTR lpCLientLogin = PCharToLPCWSTR(clientLogin);
	SendMessage(hOnlineClientsListBox, LB_ADDSTRING, 0, (LPARAM)lpCLientLogin);
	free((void*)lpCLientLogin);
}


void MainWindow::RemoveClientFromListBox(char* clientLogin)
{
	LPCWSTR lpCLientLogin = PCharToLPCWSTR(clientLogin);
	int index = SendMessage(hOnlineClientsListBox, LB_FINDSTRING, 0, (LPARAM)lpCLientLogin);
	SendMessage(hOnlineClientsListBox, LB_DELETESTRING, index, 0);
	free((void*)lpCLientLogin);
}


LRESULT CALLBACK MainWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	static MainWindow* mainWindow;
	static Client* client = Client::GetInstance();
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
			client->LeaveChat();
			mainWindow->RefreshControlsState();
			break;
		case ID_M_EXIT:
			if (client->IsOnline())
			{
				client->LeaveChat();
			}
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_CREATE:
	{
		//code	
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
	{
		PostQuitMessage(0);
		break;
	}
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
	if (hControl == hCallButton)
	{
		switch (controlMsg)
		{
		case BN_CLICKED:
		{
			char selectedClient[STR_BUFF_SIZE];
			if (GetListBoxSelectedClient(selectedClient) != LB_ERR)
			{
				char err_msg[STR_BUFF_SIZE];
				sockaddr_in destClientVideoListAddr;
				if (client->TryConnectTo(selectedClient, destClientVideoListAddr,err_msg))
				{
					//code
				}
				else
				{
					dialogManager->ShowError(err_msg);
				}
			}
			break;
		}
		default:
			break;
		}
	}
}


int MainWindow::GetListBoxSelectedClient(char *selectedClient)
{
	int itemIndex = (int)SendMessage(hOnlineClientsListBox, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (itemIndex == LB_ERR)
	{
		return itemIndex;
	}
	int textLen = (int)SendMessage(hOnlineClientsListBox, LB_GETTEXTLEN, (WPARAM)itemIndex, 0);
	TCHAR *clientToCall = new TCHAR[textLen + 1];
	SendMessage(hOnlineClientsListBox, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)clientToCall);
	CharToOem(clientToCall, selectedClient);
	free(clientToCall);
}


void MainWindow::Show()
{
	RefreshControlsState();
	Window::Show();
	//camera->Open();
	//ResumeThread(webcamThread);
	//webCamThreadSuspended = false;
}


void MainWindow::RefreshControlsState()
{
	//RefreshCapDeviceToComboBox();
	if (client->IsOnline())
	{
		char newWndTitle[1000];
		sprintf(newWndTitle, "NiceChat - %s %s", client->Name(), client->LastName());
		SetWindowTitle(newWndTitle);
		vector<ClientInfo> onlineClientsList = client->GetOnlineClientsList();
		SetOnlineClientsList(onlineClientsList);
		EnableWindow(hCallButton, TRUE);
		EnableMenuItem(hMenu, ID_M_LOGIN, MF_DISABLED);
		EnableMenuItem(hMenu, ID_M_REGISTRATE, MF_DISABLED);
		EnableMenuItem(hMenu, ID_M_LEAVE_CHAT, MF_ENABLED);
	}
	else
	{
		SendMessage(hOnlineClientsListBox, LB_RESETCONTENT, 0, 0);
		EnableWindow(hCallButton, FALSE);
		EnableMenuItem(hMenu, ID_M_LOGIN, MF_ENABLED);
		EnableMenuItem(hMenu, ID_M_REGISTRATE, MF_ENABLED);
		EnableMenuItem(hMenu, ID_M_LEAVE_CHAT, MF_DISABLED);
		SetWindowTitle("NiceChat");
	}
}


void MainWindow::SetWindowTitle(char *newWndTitle)
{
	LPCWSTR lpNewWndTitle;
	lpNewWndTitle = PCharToLPCWSTR(newWndTitle);
	SetWindowText(hWnd, lpNewWndTitle);
	free((wchar_t*)lpNewWndTitle);
}


void MainWindow::SetOnlineClientsList(vector<ClientInfo> onlineClients)
{
	int countOnlineClients = onlineClients.size();
	for (int i = 0; i < countOnlineClients; i++)
	{
		if (strcmp(onlineClients[i].login, client->Login()) != 0)
		{
			LPCWSTR lpStr = PCharToLPCWSTR(onlineClients[i].login);
			SendMessage(hOnlineClientsListBox, LB_ADDSTRING, 0, (LPARAM)lpStr);
			free((wchar_t*)lpStr);
		}
	}
}


void MainWindow::Hide()
{
	//SuspendThread(webcamThread);
	//webCamThreadSuspended = true;
	//camera->Close();
	Window::Hide();
}


void MainWindow::StartCall(sockaddr_in destVideoListAddr)
{
	hCallThread = CreateThread(NULL, 0, &(CallProc), NULL, 0, 0);
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


DWORD WINAPI CallProc(CONST LPVOID lpParam)
{
	static MainWindow* mainWnd = (MainWindow*)WindowManager::GetInstance()
		->GetWindow(WINDOW_TYPE::MAIN);
	static Client* client = mainWnd->client;
	static HWND hCamBox = mainWnd->hWebCamBox;
	static Camera* cam = mainWnd->camera;
	static const ImageProcesser* imageProcesser = mainWnd->imageProcesser;
	static HDC hCamBoxDC;
	static HDC hBuffDC = CreateCompatibleDC(NULL);
	static cv::Mat camFrame;
	static const int webCamBoxWidth = mainWnd->webCamBoxWidth;
	static const int webCamBoxHeight = mainWnd->webCamBoxHeight;
	while (client->IsOnCall())
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