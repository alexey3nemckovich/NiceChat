#include "stdafx.h"
#include "Client.h"
using namespace std;


DWORD WINAPI ServListenProc(LPVOID lParam);


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
	u_long iUdpSocketsMode = 1;
	//Server listening socket
	udp_sock_serv = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock_serv == INVALID_SOCKET)
	{
		printf("socket() error: %d\n", WSAGetLastError());
		WSACleanup();
		ExitProcess(0);
	}
	ZeroMemory(&udp_sock_serv_addr, sizeof(udp_sock_serv_addr));
	udp_sock_serv_addr.sin_family = AF_INET;
	udp_sock_serv_addr.sin_port = htons(1234);
	udp_sock_serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (::bind(udp_sock_serv, (struct sockaddr*)&udp_sock_serv_addr, sizeof(udp_sock_serv_addr)))
	{
		printf("Error bind %d\nPress 'Enter' to exit.", WSAGetLastError());
		closesocket(udp_sock_serv);
		WSACleanup();
		ExitProcess(0);
	}
	if (ioctlsocket(udp_sock_serv, FIONBIO, &iUdpSocketsMode))
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
	udp_sock_video_addr.sin_family = AF_INET;	
	udp_sock_video_addr.sin_port = htons(1324);
	udp_sock_video_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if (::bind(udp_sock_video, (sockaddr*)&udp_sock_video_addr, sizeof(udp_sock_video_addr)))
	{
		printf("Error bind %d\nPress 'Enter' to exit.", WSAGetLastError());
		closesocket(udp_sock_video);
		WSACleanup();
		ExitProcess(0);
	}
	if (ioctlsocket(udp_sock_serv, FIONBIO, &iUdpSocketsMode))
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
	online = true;
	servListenThread = CreateThread(NULL, 0, &(ServListenProc), NULL, 0, 0);
	//Check registration result
	int recv_len = 0;
	recv_len = recv(tcp_sock, buff, Client::BUFF_LEN, 0);
	closesocket(tcp_sock);
	if (recv_len == 0)
	{
		online = true;
		servListenThread = CreateThread(NULL, 0, &(ServListenProc), NULL, 0, 0);
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


DWORD WINAPI ServListenProc(LPVOID lParam)
{
	char buff[Client::BUFF_LEN];
	ZeroMemory(buff, Client::BUFF_LEN);
	Client* client = Client::GetInstance();
	sockaddr_in serv_addr;
	int serv_addr_size;
	int recv_len;
	while (client->online)
	{
		recv_len = recvfrom(client->udp_sock_serv, buff, Client::BUFF_LEN, 0, NULL, 0);
		if (recv_len != -1)
		{
			printf("Got message from serv!");
		}
	}
	return 0;
}