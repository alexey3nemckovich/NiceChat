#pragma once


#define BUFF_LEN 1000
#define SERVER_PORT 666
#define STR_BUFF_SIZE 50


#include <mutex>
#include <string>
#include <winsock2.h>
#include "ClientInfo.h"
#include "Network.h"
#include "WindowManager.h"
using namespace std;


class Client
{
private:
	//Fields
	SOCKET udp_sock_serv;
	SOCKET udp_sock_video;
	sockaddr_in udp_sock_serv_addr;
	sockaddr_in udp_sock_video_addr;
	struct sockaddr_in server_addr;
	bool online;
	HANDLE servListenThread;
	HANDLE videoListenThread;
	char name[STR_BUFF_SIZE];
	char last_name[STR_BUFF_SIZE];
	char login[STR_BUFF_SIZE];
	//Methods
	Client();
	~Client();
	void Init();
	int TrySetTCPConnectionWithServ(SOCKET *sock);
	friend DWORD WINAPI ServListenProc(LPVOID lParam);
public:
	static Client* GetInstance();
	bool TryLogin(
		const char *const login,
		const char *const pass,
		char *err_message
	);
	bool TryRegistrate(
		const char *const name,
		const char *const last_name,
		const char *const login,
		const char *const pass,
		char *err_message
	);
	bool TryConnectTo(
		const char const *destinyClient,
		sockaddr_in &destinyAddr,
		char *err_message
	);
	void LeaveChat();
	bool IsOnline()
	{
		return online;
	}
	char* Name()
	{
		return name;
	}
	char* LastName()
	{
		return last_name;
	}
	char* Login()
	{
		return login;
	}
	vector<ClientInfo> GetOnlineClientsList();
};