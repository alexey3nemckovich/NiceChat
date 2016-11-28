#pragma once
#include <WinSock2.h>


bool bindSocketWithRandomAddr(SOCKET socket, sockaddr_in* binded_addr);