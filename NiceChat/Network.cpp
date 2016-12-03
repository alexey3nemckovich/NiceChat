#include "stdafx.h"
#include "Network.h"


bool bindSocketWithRandomAddr(SOCKET socket, sockaddr_in* binded_addr)
{
	ZeroMemory(binded_addr, sizeof(*binded_addr));
	(*binded_addr).sin_family = AF_INET;
	(*binded_addr).sin_addr.s_addr = INADDR_ANY;
	bool sock_binded = false;
	while (!sock_binded)
	{
		(*binded_addr).sin_port = htons(rand() % 2000 + 1000);
		if (::bind(socket, (struct sockaddr*)binded_addr, sizeof(*binded_addr)))
		{
			int wsaLastErr = WSAGetLastError();
			if (wsaLastErr != WSAEADDRINUSE)
			{
				printf("Error bind %d\nPress 'Enter' to exit.", wsaLastErr);
				closesocket(socket);
				WSACleanup();
				return false;
			}
		}
		else
		{
			sock_binded = true;
		}
	}
	return true;
}