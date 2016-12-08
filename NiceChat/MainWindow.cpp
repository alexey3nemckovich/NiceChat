#include "stdafx.h"
#include "MainWindow.h"


MainWindow::MainWindow() 
	: Window(MainWndProc, _T("MainWindowClass"), _T("NiceChat"), 900, 600, 0)
{
	hMenu = LoadMenu(WindowManager::GetHInstance(), MAKEINTRESOURCE(IDC_NICECHAT));
	SetMenu(hWnd, hMenu);
	Init();
}


MainWindow::~MainWindow()
{
	if (hRenderWebcamThread != NULL)
	{
		isAlive = false;
		if (webCamRenderThreadSuspended)
		{
			ResumeThread(hRenderWebcamThread);
		}
		WaitForSingleObject(hRenderWebcamThread, INFINITE);
		CloseHandle(hRenderWebcamThread);
		hRenderWebcamThread = NULL;	
	}
}


void MainWindow::Init()
{
	isAlive = true;
	imageProcesser = ImageProcesser::GetInstance();
	hCallThread = NULL;
	hRenderWebcamThread = NULL;
	//
	//hRenderWebcamThread = CreateThread(NULL, 0, &(CamRenderThreadProc), NULL, CREATE_SUSPENDED, 0);
	//webCamRenderThreadSuspended = true;
	//init conrtols
	DWORD cmbBoxStyle = CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_TABSTOP;
	hListCapsComboBox = windowConstructor->CreateControl(L"COMBOBOX", L"", hWnd, webCamBoxLeft, 0, 40, 100, cmbBoxStyle);
	DWORD camBoxStyle = SS_CENTER | WS_BORDER | WS_VISIBLE | WS_CHILD /*| SS_BITMAP*/ | SS_CENTERIMAGE;
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
	DWORD callButtonStyle = 0;
	hCallButton = windowConstructor->CreateControl(
		L"BUTTON",
		L"Call",
		hWnd,
		clientsListBoxLeft,
		webCamBoxTop + clientsListBoxHeight + 10,
		clientsListBoxWidth,
		callBtnHeight,
		callButtonStyle
	);
	camera = Camera::GetInstance();
}


void MainWindow::Show()
{
	RefreshControls(client->GetStatus());
	if (client->GetStatus() == ClientStatus::Online)
	{
		RefreshOnlineClientsList();
	}
	RefreshCameraComponents();
	if (camera != NULL)
	{
		//camera->Open();
	}
	Window::Show();
	//
	//ResumeThread(hRenderWebcamThread);
	//webCamRenderThreadSuspended = false;
}


void MainWindow::Hide()
{
	//SuspendThread(webcamThread);
	//webCamThreadSuspended = true;
	if (camera != NULL)
	{
		camera->Close();
	}
	Window::Hide();
}


void MainWindow::RefreshControls(ClientStatus status, char *clientLogin)
{
	switch (status)
	{
	case ClientStatus::Offline:
	case ClientStatus::Online:
	{
		Window::FillWithBrush(hWebCamBox, (HBRUSH)GetStockObject(GRAY_BRUSH));
		Window::SetText(hWebCamBox, "");
		Window::SetText(hCallButton, "Call");
		if (status == ClientStatus::Offline)
		{
			Window::SetText(hWnd, "NiceChat");
			SendMessage(hOnlineClientsListBox, LB_RESETCONTENT, 0, 0);
			EnableWindow(hCallButton, FALSE);
			EnableMenuItem(hMenu, ID_M_LOGIN, MF_ENABLED);
			EnableMenuItem(hMenu, ID_M_REGISTRATE, MF_ENABLED);
			EnableMenuItem(hMenu, ID_M_LEAVE_CHAT, MF_DISABLED);
		}
		else
		{
			char newWndTitle[1000];
			sprintf(newWndTitle, "NiceChat - %s %s", client->Name(), client->LastName());
			Window::SetText(hWnd, newWndTitle);
			EnableWindow(hCallButton, TRUE);
			EnableMenuItem(hMenu, ID_M_LOGIN, MF_DISABLED);
			EnableMenuItem(hMenu, ID_M_REGISTRATE, MF_DISABLED);
			EnableMenuItem(hMenu, ID_M_LEAVE_CHAT, MF_ENABLED);
		}
		break;
	}
	case ClientStatus::Calling:
	{
		Window::SetText(hCallButton, "Cancel");
		char callingStr[STR_BUFF_SIZE];
		sprintf(callingStr, "Calling to '%s'...", clientLogin);
		Window::SetText(hWebCamBox, callingStr);
		break;
	}
	case ClientStatus::OnCall:
	{
		Window::SetText(hCallButton, "Finish chatting");
		break;
	}
	default:
		break;
	}
}


void MainWindow::RefreshCameraComponents()
{
	SendMessage(hListCapsComboBox, CB_RESETCONTENT, 0, 0);
	vector<int> freeCapsIndexes = Camera::GetListFreeCapsIndexes();
	for each(int capIndex in freeCapsIndexes)
	{
		AddCapDeviceIndexToComboBox(capIndex);
	}
	if (freeCapsIndexes.size())
	{
		if (!camera->IsAvailable())
		{
			camera->SetCapDeviceIndex(freeCapsIndexes[0]);
		}
	}
}


void MainWindow::RefreshOnlineClientsList()
{
	vector<ClientInfo> onlineClients = client->GetOnlineClientsList();
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


void MainWindow::ShowIncomingCall(char *callerLogin)
{
	IncomingCallWindow* callWindow = (IncomingCallWindow*)windowManager->GetWindow(WINDOW_TYPE::INCOMING_CALL);
	callWindow->SetCallerLogin(callerLogin);
	callWindow->Show();
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


int MainWindow::GetListBoxSelectedClient(char* selectedClient)
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


LRESULT CALLBACK MainWndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam
)
{
	static MainWindow* mainWindow = NULL;
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
			mainWindow->RefreshControls(client->GetStatus());
			break;
		case ID_M_EXIT:
			if (client->GetStatus() != ClientStatus::Offline)
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
		////BAD BAD VERY BAD
		mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);
		if (mainWindow->client->GetStatus() != ClientStatus::Offline)
		{
			Window::FillWithBrush(mainWindow->hWebCamBox, (HBRUSH)GetStockObject(WHITE_BRUSH));
		}
		////
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
			switch (client->GetStatus())
			{
			case ClientStatus::Online:
			{
				static char selectedClient[STR_BUFF_SIZE];
				if (GetListBoxSelectedClient(selectedClient) != LB_ERR)
				{
					CallClick(selectedClient);
				}
				break;
			}
			case ClientStatus::Calling:
			{
				CancelCallClick();			
				break;
			}
			case ClientStatus::OnCall:
			{
				FinishChattingClick();
			}
			default:
				break;
			}
			break;
		}
		default:
			break;
		}
	}
}


void MainWindow::CallClick(char *clientLogin)
{
	hCallThread = CreateThread(NULL, 0, &CallThreadProc, clientLogin, 0, 0);
	RefreshControls(ClientStatus::Calling ,clientLogin);
}


void MainWindow::CancelCallClick()
{
	client->CancelOutgoingCall();
	RefreshControls(client->GetStatus());
}


void MainWindow::FinishChattingClick()
{
	client->EndChatting();
	RefreshControls(client->GetStatus());
}


DWORD WINAPI CallThreadProc(CONST LPVOID lParam)
{
	char* destClientLogin = (char*)lParam;
	static MainWindow* mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);
	static Client* client = Client::GetInstance();
	char err_msg[STR_BUFF_SIZE];
	if (client->TryConnectTo((char*)destClientLogin, err_msg))
	{
		client->StartChatting();
		mainWindow->RefreshControls(client->GetStatus());
	}
	else
	{
		mainWindow->RefreshControls(client->GetStatus());
		DialogManager::GetInstance()->ShowError(err_msg);
	}
	return 0;
}


DWORD WINAPI CamRenderThreadProc(CONST LPVOID lParam)
{
	static MainWindow* mainWnd = (MainWindow*)WindowManager::GetInstance()
		->GetWindow(WINDOW_TYPE::MAIN);
	static Camera* cam = Camera::GetInstance();
	static CamFrame camFrame;
	while (mainWnd->isAlive)
	{
		camFrame = cam->GetFrame();
		mainWnd->RenderFrame(camFrame);
	}
	return 0;
}


void MainWindow::RenderFrame(CamFrame camFrame)
{
	static HDC hBuffDC = CreateCompatibleDC(NULL);
	static HDC hCamBoxDC;
	HBITMAP hBmpFrame = imageProcesser->ConvertCVMatToHBITMAP(camFrame.img);
	SelectObject(hBuffDC, hBmpFrame);
	hCamBoxDC = GetDC(hWebCamBox);
	BitBlt(hCamBoxDC, 0, 0, webCamBoxWidth, webCamBoxHeight, hBuffDC, 0, 0, SRCCOPY);
	ReleaseDC(hWebCamBox, hCamBoxDC);
}