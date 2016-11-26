#pragma once
#include <mutex>
#include <string>
#include <winsock2.h>
using namespace std;


class Client
{
private:
	//Methods
	Client();
	~Client();
	void Init();
	//Fields
	SOCKET udp_sock;
	const int SERVER_PORT = 666;
	static const int BUFF_LEN = 1000000;
	struct sockaddr_in server_addr;
	bool connected_server;
public:
	static Client* GetInstance();
	bool TryLogin(
		const char *const login,
		const char *const pass,
		const char *err_message
	);
	bool TryRegistrate(
		const char *const name,
		const char *const last_name,
		const char *const login,
		const char *const pass,
		const char *err_message
	);
	void ConnectTo(string login);
	void LeaveChat();
};