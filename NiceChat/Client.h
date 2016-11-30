#pragma once


#define BUFF_LEN 1000
#define SERVER_TCP_PORT 666
#define SERVER_UDP_PORT 777
#define STR_BUFF_SIZE 50
#define CALL_ACCEPT_STR "accept"


#include <mutex>
#include <string>
#include <winsock2.h>
#include <opencv2\opencv.hpp>
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
	sockaddr_in interlocutor_sock_addr;
	bool online;
	HANDLE hServListenThread;
	HANDLE hSendFrameThread;
	HANDLE hRecvFrameThread;
	char name[STR_BUFF_SIZE];
	char last_name[STR_BUFF_SIZE];
	char login[STR_BUFF_SIZE];
	bool onCall = false;
	//Methods
	Client();
	~Client();
	void Init();
	int TrySetTCPConnectionWithServ(SOCKET *sock);
	friend DWORD WINAPI ServListenProc(LPVOID lParam);
	friend DWORD WINAPI SendFrameThreadProc(LPVOID lpParam);
	friend DWORD WINAPI RecvFrameThreadProc(LPVOID lpParam);
	friend void IncomingCall(SOCKET udp_sock_serv);
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
		char *err_message
	);
	void StartVideoExchange();
	void EndVideoExchange();
	void SendFrame(cv::Mat frame);
	cv::Mat RecvFrame();
	void LeaveChat();
	bool IsOnline()
	{
		return online;
	}
	bool IsOnCall()
	{
		return onCall;
	}
	void SetOnCall()
	{
		onCall = true;
	}
	void SetFree()
	{
		onCall = false;
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
	static sockaddr_in serv_tcp_addr;
	static sockaddr_in serv_udp_addr;
};