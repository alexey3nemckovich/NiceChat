#pragma once
#include <mutex>
#include <string>
#include <winsock2.h>
#include "ClientInfo.h"
#include "Network.h"
using namespace std;


class Client
{
private:
	//Fields
	const int SERVER_PORT = 666;
	static const int BUFF_LEN = 1000000;
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
	bool TryConnectTo(char *login);
	void LeaveChat();
};