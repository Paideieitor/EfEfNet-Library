#include "efefGeneral.h"

#include "efefPrivateGlobals.h"
#include "efefManager.h"

int efef::Init()
{
    WORD version = MAKEWORD(2, 2);
    WSADATA data;

    if (WSAStartup(version, &data) == SOCKET_ERROR)
        return efef::DebugError(L"Start Up Error");

    if (data.wVersion != version)
    {
        if (WSACleanup() == SOCKET_ERROR)
            return efef::DebugError(L"Version Clean Up Error");
        return efef::DebugError(L"Version Error");
    }

    if (efef::manager::inst == nullptr)
        efef::manager::inst = new manager();

    return EFEF_NO_ERROR;
}

int efef::CleanUp()
{
    if (WSACleanup() == SOCKET_ERROR)
        return efef::DebugError(L"Clean Up Error");

    delete efef::manager::inst;
    efef::manager::inst = nullptr;

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
        return efef::DebugError(L"Create UDP Socket Error");

    return udp_socket((uint)newSocket);
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
        return efef::DebugError(L"Create TCP Socket Error");

    return tcp_socket((uint)newSocket);
}

efef::fast_socket efef::CreateFastSocket(address_family family)
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
        return efef::DebugError(L"Create Fast Socket Error");

    return fast_socket((uint)newSocket);
}

efef::fast_socket* efef::CreateDynmcFastSocket(address_family family)
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
    {
        efef::DebugError(L"Create Fast Socket Error");
        return nullptr;
    }

    return new fast_socket((uint)newSocket);
}

int efef::Poll(udp_socket* const socket, select_mode mode, long millisec)
{
    udp_set set;
    set.add(socket);

    int error = priv_util::Select(&set, mode, millisec);
    if (error < 0)
        return EFEF_ERROR;

    return set.size();
}

int efef::Select(udp_set* const sockets, select_mode mode, long millisec)
{
    return priv_util::Select(sockets, mode, millisec);
}

int efef::Select(udp_set* const receive, udp_set* const send, udp_set* const except, long millisec)
{
    return priv_util::Select(receive, send, except, millisec);
}

int efef::Poll(tcp_socket* const socket, select_mode mode, long millisec)
{
    tcp_set set;
    set.add(socket);

    int error = priv_util::Select(&set, mode, millisec);
    if (error != 0)
        return EFEF_ERROR;

    return set.size();
}

int efef::Select(tcp_set* const sockets, select_mode mode, long millisec)
{
    return priv_util::Select(sockets, mode, millisec);
}

int efef::Select(tcp_set* const receive, tcp_set* const send, tcp_set* const except, long millisec)
{
    return priv_util::Select(receive, send, except, millisec);
}


