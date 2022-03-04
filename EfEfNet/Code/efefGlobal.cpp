#include "efefGlobal.h"

sockaddr* efef::GetAddress(address& addr)
{
	return reinterpret_cast<sockaddr*>(&addr);
}

sockaddr_in* efef::GetIPv4(address& addr)
{
	return reinterpret_cast<sockaddr_in*>(&addr);
}
