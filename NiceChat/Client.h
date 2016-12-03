#pragma once


#define BUFF_LEN 1000
#define FRAME_BUFF_LEN 65000
#define SERVER_IP "192.168.100.3"
#define SERVER_TCP_PORT 666
#define SERVER_UDP_PORT 777
#define STR_BUFF_SIZE 50
#define CALL_ACCEPT_STR "accept"
#define CALL_CANCEL_STR "cancel"


#include <mutex>
#include <string>
#include <winsock2.h>
#include <opencv2\opencv.hpp>
#include "ClientInfo.h"
#include "Network.h"
#include "Camera.h"
#include "WindowManager.h"
using namespace std;


enum class ClientStatus {
	Offline,
	Online,
	Calling,
	Connected,
	OnCall
};


class Client
{
private:
	//Fields
	SOCKET udp_sock_serv;
	SOCKET udp_sock_video;
	sockaddr_in udp_sock_serv_addr;
	sockaddr_in udp_sock_video_addr;
	sockaddr_in interlocutor_sock_addr;
	HANDLE hServListenThread;
	HANDLE hSendFrameThread;
	HANDLE hRecvFrameThread;
	char name[STR_BUFF_SIZE];
	char last_name[STR_BUFF_SIZE];
	char login[STR_BUFF_SIZE];
	ClientStatus status;
	//Methods
	Client();
	~Client();
	void Init();
	void SendFrame(CamFrame frame);
	CamFrame RecvFrame();
	void StartVideoExchange();
	void EndVideoExchange();
	int TrySetTCPConnectionWithServ(SOCKET *sock);
	//friend methods
	friend DWORD WINAPI ServListenProc(LPVOID lParam);
	friend DWORD WINAPI SendFrameThreadProc(LPVOID lpParam);
	friend DWORD WINAPI RecvFrameThreadProc(LPVOID lpParam);
	friend void IncomingCall(SOCKET udp_sock_serv);
	friend void ClientLeaved(SOCKET udp_sock_serv);
	friend void ClientJoined(SOCKET udp_sock_serv);
	friend void CallEnd(SOCKET udp_sock_serv);
	friend void StartChat(SOCKET udp_sock_serv);
public:
	//Fields
	static sockaddr_in serv_tcp_addr;
	static sockaddr_in serv_udp_addr;
	//Methods
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
		char *destinyClient,
		char *err_message
	);
	void AcceptIncomingCall();
	void CancelIncomingCall();
	void CancelOutgoingCall();
	void StartChatting();
	void EndChatting();
	void LeaveChat();
	ClientStatus GetStatus()
	{
		return status;
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