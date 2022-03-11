#pragma once

#define EFEF_DEBUG

#include "efefGlobals.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace efef
{
	typedef char address[ADDR_SIZE];

	sockaddr* GetAddress(address& addr);
	sockaddr_in* GetIPv4(address& addr);
}

// Winsock socket function wrap
// this allows to name the fuctions on my classes the same as the winsock functions
int ws2bind(SOCKET s, const sockaddr* name, int namelen);
int ws2connect(SOCKET s, const sockaddr* name, int namelen);
int ws2listen(SOCKET s, int backlog);
SOCKET ws2accept(SOCKET s, sockaddr* addr, int* addrlen);
int ws2send(SOCKET s, const char* buf, int len, int flags);
