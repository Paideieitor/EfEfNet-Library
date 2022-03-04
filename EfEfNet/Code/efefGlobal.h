#pragma once

#define EFEF_DEBUG

#include "efefTypes.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

namespace efef
{
	typedef char address[ADDR_SIZE];

	sockaddr* GetAddress(address& addr);
	sockaddr_in* GetIPv4(address& addr);
}
