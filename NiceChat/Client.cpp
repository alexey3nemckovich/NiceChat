#include "stdafx.h"
#include "Client.h"
using namespace std;


Client::Client()
{
	Init();
}


void Client::Init()
{
	connected_server = false;
	//Initialise winsock lib
	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData))
	{
		printf("WSAStart error %d\n", WSAGetLastError());
		ExitProcess(0);
	}
	//Create sockets
	udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (udp_sock == INVALID_SOCKET)
	{
		printf("socket() error: %d\n", WSAGetLastError());
		WSACleanup();
		ExitProcess(0);
	}
	//Init server sock addr
	char *SERVERADDR = "127.0.0.1";
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
}


Client::~Client()
{
	closesocket(udp_sock);
	WSACleanup();
}


Client* Client::GetInstance()
{
	static Client client;
	return &client;
}


bool Client::TryLogin(
	const char *const login,
	const char *const pass,
	const char *err_message
)
{
	return true;
}


bool Client::TryRegistrate(
	const char *const name,
	const char *const last_name,
	const char *const login,
	const char *const pass,
	const char *err_message
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
	connected_server = true;
	buff[0] = 0;
	send(tcp_sock, buff, 1, 0);
	int mess_len = strlen(name);
	Sleep(5);
	send(tcp_sock, name, mess_len, 0);
	Sleep(5);
	send(tcp_sock, last_name, strlen(last_name), 0);
	Sleep(5);
	send(tcp_sock, login, strlen(login), 0);
	Sleep(5);
	send(tcp_sock, pass, strlen(pass), 0);
	closesocket(tcp_sock);
	return true;
}


void Client::LeaveChat()
{

}