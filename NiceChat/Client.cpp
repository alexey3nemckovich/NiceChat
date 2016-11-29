#include "stdafx.h"
#include "Client.h"
#include "MainWindow.h"
using namespace std;


DWORD WINAPI ServListenProc(LPVOID lParam);
u_long I_BLOCKING_SOCKETS_MODE = FALSE;
u_long I_NON_BLOCKING_SOCKETS_MODE = TRUE;
MainWindow* mainWindow = (MainWindow*)WindowManager::GetInstance()->GetWindow(WINDOW_TYPE::MAIN);


Client::Client()
{
	Init();
}


Client::~Client()
{
	if (online)
	{
		LeaveChat();
	}
	closesocket(udp_sock_serv);
	closesocket(udp_sock_video);
	WSACleanup();
}


void Client::Init()
{
	online = false;
	servListenThread = NULL;
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
	if (ioctlsocket(udp_sock_serv, FIONBIO, &(I_NON_BLOCKING_SOCKETS_MODE)))
	{
		ExitProcess(0);
	}
	//Init server sock addr
	char *SERVERADDR = "127.0.0.1";
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
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
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	char buff[BUFF_LEN];
	ZeroMemory(buff, BUFF_LEN);
	if (connect(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		int connect_err = WSAGetLastError();
		return false;
	}
	buff[0] = 1;
	send(tcp_sock, buff, 1, 0);
	Sleep(5);
	send(tcp_sock, login, strlen(login), 0);
	Sleep(5);
	send(tcp_sock, pass, strlen(pass), 0);
	Sleep(5);
	recv(tcp_sock, buff, BUFF_LEN, 0);
	//check
	closesocket(tcp_sock);
	online = true;
	servListenThread = CreateThread(NULL, 0, &(ServListenProc), NULL, 0, 0);
	return true;
}


bool Client::TryRegistrate(
	const char *const name,
	const char *const last_name,
	const char *const login,
	const char *const pass,
	char *err_message
)
{
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	char buff[BUFF_LEN];
	ZeroMemory(buff, BUFF_LEN);
	if (connect(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		int connect_err = WSAGetLastError();
		return false;
	}
	//Send to server own properties
	buff[0] = 0;
	send(tcp_sock, buff, 1, 0);
	Sleep(5);
	send(tcp_sock, name, strlen(name), 0);
	Sleep(5);
	send(tcp_sock, last_name, strlen(last_name), 0);
	Sleep(5);
	send(tcp_sock, login, strlen(login), 0);
	Sleep(5);
	send(tcp_sock, pass, strlen(pass), 0);
	Sleep(5);
	//Send to server udp sockets addresses
	send(tcp_sock, (char*)(&udp_sock_serv_addr), sizeof(udp_sock_serv_addr), 0);
	Sleep(5);
	send(tcp_sock, (char*)(&udp_sock_video_addr), sizeof(udp_sock_video_addr), 0);
	Sleep(5);
	//Check registration result
	int recv_len = 0;
	recv_len = recv(tcp_sock, buff, BUFF_LEN, 0);
	closesocket(tcp_sock);
	if (recv_len == 0)
	{
		online = true;
		servListenThread = CreateThread(NULL, 0, &(ServListenProc), NULL, 0, 0);
		strcpy(this->name, name);
		strcpy(this->last_name, last_name);
		strcpy(this->login, login);
		return true;
	}
	else
	{
		strcpy(err_message, buff);
		return false;
	}
	//
}


bool Client::TryConnectTo(char *login)
{
	return true;
}


void Client::LeaveChat()
{
	online = false;
	WaitForSingleObject(servListenThread, INFINITE);
}


vector<ClientInfo> Client::GetOnlineClientsList()
{
	SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
	char buff[BUFF_LEN];
	ZeroMemory(buff, BUFF_LEN);
	connect(tcp_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	buff[0] = 4;
	send(tcp_sock, buff, 1, 0);
	Sleep(5);
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


void IncomingCall(SOCKET udp_sock_serv);
void ClientLeaved(SOCKET udp_sock_serv);
void ClientJoined(SOCKET udp_sock_serv);


/*
@Separate thread procedure for processing events from server.
@It starts, when client joins chat(logined or recently registrated).
@First message from server says kind of event:
@	0 - call from other client
@	1 - some client leaved chat
@	2 - new client joined chat
*/
DWORD WINAPI ServListenProc(LPVOID lParam)
{
	char buff[BUFF_LEN];
	ZeroMemory(buff, BUFF_LEN);
	Client* client = Client::GetInstance();
	SOCKET udp_sock_serv = client->udp_sock_serv;
	/*sockaddr_in serv_addr;
	int serv_addr_size;*/
	int recv_len;
	while (client->online)
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

}


void ClientLeaved(SOCKET udp_sock_serv)
{

}


void ClientJoined(SOCKET udp_sock_serv)
{
	char buff[BUFF_LEN];
	//recv joined client login
	recvfrom(udp_sock_serv, buff, BUFF_LEN, 0, NULL, 0);
	mainWindow->AddClientToListBox(buff);
}