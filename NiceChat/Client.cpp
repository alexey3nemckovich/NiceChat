#include "stdafx.h"
#include "Client.h"
#include "MainWindow.h"
using namespace std;


sockaddr_in Client::serv_tcp_addr;
sockaddr_in Client::serv_udp_addr;


DWORD WINAPI ServListenProc(LPVOID lParam);
unsigned long I_BLOCKING_SOCKETS_MODE = FALSE;
unsigned long I_NON_BLOCKING_SOCKETS_MODE = TRUE;
MainWindow* mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);
Client* client = Client::GetInstance();


Client::Client()
{
	Init();
}


Client::~Client()
{
	if (status != ClientStatus::Offline)
	{
		LeaveChat();
	}
	closesocket(udp_sock_serv);
	closesocket(udp_sock_video);
	WSACleanup();
}


void Client::Init()
{
	status = ClientStatus::Offline;
	hServListenThread = NULL;
	hSendFrameThread = NULL;
	hRecvFrameThread = NULL;
	//Initialise winsock lib
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		ExitProcess(0);
	}
	//Server listening socket
	udp_sock_serv = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock_serv == INVALID_SOCKET)
	{
		printf("socket() error: %d\n", WSAGetLastError());
		WSACleanup();
		ExitProcess(0);
	}
	if (!bindSocketWithRandomAddr(udp_sock_serv, &udp_sock_serv_addr))
	{
		ExitProcess(0);
	}
	//Enable non blocking socket mode
	if (ioctlsocket(udp_sock_serv, FIONBIO, &(I_NON_BLOCKING_SOCKETS_MODE)))
	{
		ExitProcess(0);
	}
	//Video frames listening socket
	udp_sock_video = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock_video == INVALID_SOCKET)
	{
		printf("socket() error: %d\n", WSAGetLastError());
		WSACleanup();
		ExitProcess(0);
	}
	if (!bindSocketWithRandomAddr(udp_sock_video, &udp_sock_video_addr))
	{
		ExitProcess(0);
	}
	//Enable non blocking socket mode
	if (ioctlsocket(udp_sock_video, FIONBIO, &(I_NON_BLOCKING_SOCKETS_MODE)))
	{
		ExitProcess(0);
	}
	//Init server sock addr
	serv_tcp_addr.sin_family = serv_udp_addr.sin_family = AF_INET;
	serv_tcp_addr.sin_addr.s_addr = serv_udp_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serv_tcp_addr.sin_port = htons(SERVER_TCP_PORT);
	serv_udp_addr.sin_port = htons(SERVER_UDP_PORT);
}


Client* Client::GetInstance()
{
	static Client client;
	return &client;
}


bool Client::TryLogin(
	const char *const login,
	const char *const pass,
	char *err_message
)
{
	SOCKET tcp_sock;
	if (TrySetTCPConnectionWithServ(&tcp_sock))
	{
		sprintf(err_message, "Failed to connect to server.");
		return false;
	}
	char operationNumber = 1;
	//send login and pass
	send(tcp_sock, &operationNumber, sizeof(operationNumber), 0);
	Sleep(50);
	send(tcp_sock, login, strlen(login) + 1, 0);
	Sleep(50);
	send(tcp_sock, pass, strlen(pass) + 1, 0);
	Sleep(50);
	//Send to server udp sockets addresses
	send(tcp_sock, (char*)(&udp_sock_serv_addr.sin_port), sizeof(udp_sock_serv_addr.sin_port), 0);
	Sleep(50);
	send(tcp_sock, (char*)(&udp_sock_video_addr.sin_port), sizeof(udp_sock_video_addr.sin_port), 0);
	Sleep(50);
	//Check logination result
	char buff[BUFF_LEN];
	recv(tcp_sock, buff, BUFF_LEN, 0);
	if (strcmp(buff, "ok") == 0)
	{
		recv(tcp_sock, buff, BUFF_LEN, 0);
		strcpy(name, buff);
		recv(tcp_sock, buff, BUFF_LEN, 0);
		strcpy(last_name, buff);
		strcpy(this->login, login);
		hServListenThread = CreateThread(NULL, 0, &(ServListenProc), NULL, 0, 0);
		closesocket(tcp_sock);
		status = ClientStatus::Online;
		return true;
	}
	else
	{
		closesocket(tcp_sock);
		strcpy(err_message, buff);
		return false;
	}
}


bool Client::TryRegistrate(
	const char *const name,
	const char *const last_name,
	const char *const login,
	const char *const pass,
	char *err_message
)
{
	SOCKET tcp_sock;
	if (TrySetTCPConnectionWithServ(&tcp_sock))
	{
		sprintf(err_message, "Failed to connect to server.");
		return false;
	}
	//Send to server own properties
	char operationNumber = 0;
	send(tcp_sock, &operationNumber, sizeof(operationNumber), 0);
	Sleep(50);
	send(tcp_sock, name, strlen(name) + 1, 0);
	Sleep(50);
	send(tcp_sock, last_name, strlen(last_name) + 1, 0);
	Sleep(50);
	send(tcp_sock, login, strlen(login) + 1, 0);
	Sleep(50);
	send(tcp_sock, pass, strlen(pass) + 1, 0);
	Sleep(50);
	//Send to server udp sockets addresses
	send(tcp_sock, (char*)(&udp_sock_serv_addr.sin_port), sizeof(udp_sock_serv_addr.sin_port), 0);
	Sleep(50);
	send(tcp_sock, (char*)(&udp_sock_video_addr.sin_port), sizeof(udp_sock_video_addr.sin_port), 0);
	Sleep(50);
	//Check registration result
	char buff[BUFF_LEN];
	int recv_len = 0;
	recv_len = recv(tcp_sock, buff, BUFF_LEN, 0);
	closesocket(tcp_sock);
	if (recv_len == 0)
	{
		hServListenThread = CreateThread(NULL, 0, &(ServListenProc), NULL, 0, 0);
		strcpy(this->name, name);
		strcpy(this->last_name, last_name);
		strcpy(this->login, login);
		status = ClientStatus::Online;
		return true;
	}
	else
	{
		strcpy(err_message, buff);
		return false;
	}
}


bool Client::TryConnectTo(char *destinyClient, char *err_message)
{
	status = ClientStatus::Calling;
	SOCKET tcp_sock;
	if (TrySetTCPConnectionWithServ(&tcp_sock))
	{
		sprintf(err_message, "Failed to connect to server.");
		status = ClientStatus::Online;
		return false;
	}
	char operationNumber = 2;
	send(tcp_sock, &operationNumber, sizeof(operationNumber), 0);
	Sleep(50);
	send(tcp_sock, login, strlen(login) + 1, 0);
	Sleep(50);
	char buff[BUFF_LEN];
	send(tcp_sock, destinyClient, strlen(destinyClient) + 1, 0);
	recv(tcp_sock, buff, BUFF_LEN, 0);
	if (strcmp(buff, CALL_ACCEPT_STR) == 0)
	{
		status = ClientStatus::Connected;
		recv(tcp_sock, buff, BUFF_LEN, 0);
		interlocutor_sock_addr = ((sockaddr_in*)buff)[0];
		return true;
	}
	else
	{
		status = ClientStatus::Online;
		strcpy(err_message, buff);
		return false;
	}
}


void Client::AcceptIncomingCall()
{
	//notify server about call accept
	sendto(udp_sock_serv,
		CALL_ACCEPT_STR,
		strlen(CALL_ACCEPT_STR) + 1,
		0,
		(sockaddr*)(&Client::serv_udp_addr),
		sizeof(Client::serv_udp_addr)
	);
	status = ClientStatus::Connected;
}


void Client::CancelIncomingCall()
{
	//notify server about call cancel
	sendto(udp_sock_serv,
		CALL_CANCEL_STR,
		strlen(CALL_CANCEL_STR) + 1,
		0,
		(sockaddr*)(&Client::serv_udp_addr),
		sizeof(Client::serv_udp_addr)
	);
	status = ClientStatus::Online;
}


void Client::CancelOutgoingCall()
{

}


void Client::StartChatting()
{
	status = ClientStatus::OnCall;
	StartVideoExchange();
}


void Client::EndChatting()
{
	status = ClientStatus::Online;
	//notify server about call end
	SOCKET tcp_sock;
	TrySetTCPConnectionWithServ(&tcp_sock);
	char operationNumber = 5;
	send(tcp_sock, &operationNumber, sizeof(operationNumber), 0);
	Sleep(50);
	send(tcp_sock, login, strlen(login) + 1, 0);
	//finish video exchange threads
	EndVideoExchange();
}


void Client::StartVideoExchange()
{
	hSendFrameThread = CreateThread(NULL, 0, &SendFrameThreadProc, NULL, 0, 0);
	hRecvFrameThread = CreateThread(NULL, 0, &RecvFrameThreadProc, NULL, 0, 0);
}


void Client::EndVideoExchange()
{
	WaitForSingleObject(hSendFrameThread, INFINITE);
	WaitForSingleObject(hRecvFrameThread, INFINITE);
}


void Client::SendFrame(CamFrame frame)
{
	strcpy((char*)frame.data, "123");
	frame.size = strlen((char*)frame.data);
	sendto(udp_sock_video, (char*)frame.data, frame.size, 0, (sockaddr*)&interlocutor_sock_addr, sizeof(interlocutor_sock_addr));
}


CamFrame Client::RecvFrame()
{
	static CamFrame frame;
	static uchar frameBuff[FRAME_BUFF_LEN];
	frame.data = frameBuff;
	frame.size = recvfrom(udp_sock_video, (char*)frameBuff, FRAME_BUFF_LEN, 0, NULL, 0);
	return frame;
}


void Client::LeaveChat()
{
	status = ClientStatus::Offline;
	SOCKET tcp_sock;
	if (TrySetTCPConnectionWithServ(&tcp_sock) == 0)
	{
		char operationNumber = 3;
		send(tcp_sock, &operationNumber, sizeof(operationNumber), 0);
		Sleep(50);
		send(tcp_sock, login, strlen(login) + 1, 0);
	}
	if (status == ClientStatus::OnCall)
	{
		WaitForSingleObject(hSendFrameThread, INFINITE);
		WaitForSingleObject(hRecvFrameThread, INFINITE);
	}
	WaitForSingleObject(hServListenThread, INFINITE);
}


vector<ClientInfo> Client::GetOnlineClientsList()
{
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	char buff[BUFF_LEN];
	ZeroMemory(buff, BUFF_LEN);
	//mode
	connect(tcp_sock, (struct sockaddr*)&serv_tcp_addr, sizeof(serv_tcp_addr));
	buff[0] = 4;
	send(tcp_sock, buff, 1, 0);
	Sleep(50);
	recv(tcp_sock, buff, BUFF_LEN, 0);
	int countOnlineClients = ((int*)buff)[0];
	ClientInfo clientInfo;
	vector<ClientInfo> onlineClients;
	for (int i = 0; i < countOnlineClients; i++)
	{
		recv(tcp_sock, buff, BUFF_LEN, 0);
		strcpy(clientInfo.login, buff);
		onlineClients.push_back(clientInfo);
	}
	closesocket(tcp_sock);
	return onlineClients;
}


int Client::TrySetTCPConnectionWithServ(SOCKET *sock)
{
	(*sock) = socket(AF_INET, SOCK_STREAM, 0);
	if (connect((*sock), (struct sockaddr*)&serv_tcp_addr, sizeof(serv_tcp_addr)))
	{
		return WSAGetLastError();
	}
	return 0;
}


void IncomingCall(SOCKET udp_sock_serv);
void ClientLeaved(SOCKET udp_sock_serv);
void ClientJoined(SOCKET udp_sock_serv);
void CallEnd(SOCKET udp_sock_serv);
void StartChat(SOCKET udp_sock_serv);


/*
@Separate thread procedure for processing events from server.
@It starts, when client joins chat(logined or recently registrated).
@First message from server says kind of event:
@	0 - call from other client
@	1 - some client leaved chat
@	2 - new client joined chat
@   3 - call end
@   4 - start chat to addr
*/
DWORD WINAPI ServListenProc(LPVOID lParam)
{
	char buff[BUFF_LEN];
	ZeroMemory(buff, BUFF_LEN);
	Client* client = Client::GetInstance();
	SOCKET udp_sock_serv = client->udp_sock_serv;
	int recv_len;
	while (client->status != ClientStatus::Offline)
	{
		recv_len = recvfrom(udp_sock_serv, buff, BUFF_LEN, 0, NULL, 0);
		if (recv_len != -1)
		{
			ioctlsocket(udp_sock_serv, FIONBIO, &(I_BLOCKING_SOCKETS_MODE));
			char eventNumber = buff[0];
			switch (eventNumber)
			{
			case 0:
				IncomingCall(udp_sock_serv);
				break;
			case 1:
				ClientLeaved(udp_sock_serv);
				break;
			case 2:
				ClientJoined(udp_sock_serv);
				break;
			case 3:
				CallEnd(udp_sock_serv);
				break;
			case 4:
				StartChat(udp_sock_serv);
				break;
			default:
				break;
			}
			ioctlsocket(udp_sock_serv, FIONBIO, &(I_NON_BLOCKING_SOCKETS_MODE));
		}
	}
	return 0;
}


void IncomingCall(SOCKET udp_sock_serv)
{
	static char buff[BUFF_LEN];
	static Client* client = Client::GetInstance();
	recvfrom(udp_sock_serv, buff, BUFF_LEN, 0, NULL, 0);
	char *callerLogin = buff;
	mainWindow->ShowIncomingCall(callerLogin);
}


void ClientLeaved(SOCKET udp_sock_serv)
{
	char buff[BUFF_LEN];
	//recv leaved client login
	recvfrom(udp_sock_serv, buff, BUFF_LEN, 0, NULL, 0);
	mainWindow->RemoveClientFromListBox(buff);
}


void ClientJoined(SOCKET udp_sock_serv)
{
	char buff[BUFF_LEN];
	//recv joined client login
	recvfrom(udp_sock_serv, buff, BUFF_LEN, 0, NULL, 0);
	mainWindow->AddClientToListBox(buff);
}


void CallEnd(SOCKET udp_sock_serv)
{
	client->status = ClientStatus::Online;
	mainWindow->RefreshControls(client->status);
}


void StartChat(SOCKET udp_sock_serv)
{
	client->status = ClientStatus::OnCall;
	char buff[BUFF_LEN];
	recvfrom(udp_sock_serv, buff, BUFF_LEN, 0, NULL, 0);
	sockaddr_in interlocutor_sock_addr = ((sockaddr_in*)buff)[0];
	client->interlocutor_sock_addr = interlocutor_sock_addr;
	client->StartVideoExchange();
	mainWindow->RefreshControls(client->status);
}


DWORD WINAPI SendFrameThreadProc(LPVOID lpParam)
{
	static Camera* cam = Camera::GetInstance();
	static Client* client = Client::GetInstance();
	static CamFrame frame;
	//cam->Open();
	while (client->status == ClientStatus::OnCall)
	{
		frame = cam->GetFrame();
		frame.data = (uchar*)malloc(100);
		client->SendFrame(frame);
		free((void*)frame.data);
	}
	//cam->Close();
	return 0;
}


DWORD WINAPI RecvFrameThreadProc(LPVOID lpParam)
{
	static MainWindow* mainWnd = (MainWindow*)WindowManager::GetInstance()
		->GetWindow(WINDOW_TYPE::MAIN);
	static Client* client = Client::GetInstance();
	static CamFrame frame;
	while (client->status == ClientStatus::OnCall)
	{
		frame = client->RecvFrame();
		//mainWnd->RenderFrame(frame.data);
		if (frame.size != -1)
		{
			/*LPCWSTR lpStr = PCharToLPCWSTR((char*)frame.data);
			MessageBox(NULL, lpStr, L"Mess", 0);
			free((void*)lpStr);*/
			Sleep(1000);
		}
	}
	return 0;
}