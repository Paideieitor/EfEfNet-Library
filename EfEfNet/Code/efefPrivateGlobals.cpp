#include "efefPrivateGlobals.h"

sockaddr* efef::GetAddress(address& addr)
{
	return reinterpret_cast<sockaddr*>(&addr);
}

sockaddr_in* efef::GetIPv4(address& addr)
{
	return reinterpret_cast<sockaddr_in*>(&addr);
}

int ws2bind(SOCKET s, const sockaddr* name, int namelen) { return bind(s, name, namelen); }

int ws2connect(SOCKET s, const sockaddr* name, int namelen) { return connect(s, name, namelen); }

int ws2listen(SOCKET s, int backlog) { return listen(s, backlog); }

SOCKET ws2accept(SOCKET s, sockaddr* addr, int* addrlen) { return accept(s, addr, addrlen); }

int ws2send(SOCKET s, const char* buf, int len, int flags) { return send(s, buf, len, flags); }
