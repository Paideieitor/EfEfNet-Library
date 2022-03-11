#include "efefGeneral.h"

#include "efefPrivateGlobals.h"
#include "efefDebug.h"

int efef::Init()
{
    WORD version = MAKEWORD(2, 2);
    WSADATA data;

    if (WSAStartup(version, &data) == SOCKET_ERROR)
        return efef::DebugError("Start Up Error");

    if (data.wVersion != version)
    {
        if (WSACleanup() == SOCKET_ERROR)
            return efef::DebugError("Version Clean Up Error");
        return efef::DebugError("Version Error");
    }

    return EFEF_NO_ERROR;
}

int efef::CleanUp()
{
    if (WSACleanup() == SOCKET_ERROR)
        return efef::DebugError("Clean Up Error");
    return EFEF_NO_ERROR;
}

int efef::GetLastError()
{
    return WSAGetLastError();
}

efef::udp_socket efef::CreateUDPSocket(address_family family)
{
    int af = 0;
    switch (family)
    {
    case efef::IPv4:
        af = AF_INET;
        break;
    case efef::IPv6:
        af = AF_INET6;
        break;
    }

    SOCKET newSocket = socket(af, SOCK_DGRAM, IPPROTO_UDP);
    if (newSocket == INVALID_SOCKET)
        return efef::DebugError("Create UDP Socket Error");

    return udp_socket(newSocket);
}

efef::tcp_socket efef::CreateTCPSocket(address_family family)
{
    int af = 0;
    switch (family)
    {
    case efef::IPv4:
        af = AF_INET;
        break;
    case efef::IPv6:
        af = AF_INET6;
        break;
    }

    SOCKET newSocket = socket(af, SOCK_STREAM, IPPROTO_TCP);
    if (newSocket == INVALID_SOCKET)
        return efef::DebugError("Create TCP Socket Error");

    return tcp_socket(newSocket);
}
