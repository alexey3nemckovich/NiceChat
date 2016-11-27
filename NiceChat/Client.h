#pragma once
#include <mutex>
#include <string>
#include <winsock2.h>
using namespace std;


class Client
{
private:
	//Fields
	sockaddr_in udp_sock_serv_addr;
	SOCKET udp_sock_serv;
	sockaddr_in udp_sock_video_addr;
	SOCKET udp_sock_video;
	const int SERVER_PORT = 666;
	static const int BUFF_LEN = 1000000;
	struct sockaddr_in server_addr;
	bool online;
	HANDLE servListenThread;
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
	bool TryConnectTo(char *login);
	void LeaveChat();
};