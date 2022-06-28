#include "efefSocketAddr.h"

#include "efefPrivateGlobals.h"

efef::socket_addr::socket_addr(const uint u)
{
    fill(u);
}

efef::socket_addr::socket_addr(const ulong address, const ushort port)
{
    fill(address, port);
}

efef::socket_addr::socket_addr(const int b1, const int b2, const int b3, const int b4, const ushort port)
{
    fill(b1, b2, b3, b4, port);
}

efef::socket_addr::socket_addr(wstr presentation, const ushort port)
{
    fill(presentation, port);
}

void efef::socket_addr::fill(const uint u)
{
    memory_set(mAddress, u, ADDR_SIZE);
}

void efef::socket_addr::fill(const ulong address, const ushort port)
{
    sockaddr_in* inAddress = efef::GetIPv4(mAddress);

    inAddress->sin_family = AF_INET;
    inAddress->sin_addr.S_un.S_addr = htonl(address);
    inAddress->sin_port = htons(port);
}

void efef::socket_addr::fill(const int b1, const int b2, const int b3, const int b4, const ushort port)
{
    sockaddr_in* inAddress = efef::GetIPv4(mAddress);

    inAddress->sin_family = AF_INET;
    inAddress->sin_port = htons(port);

    inAddress->sin_addr.S_un.S_un_b.s_b1 = b1;
    inAddress->sin_addr.S_un.S_un_b.s_b2 = b2;
    inAddress->sin_addr.S_un.S_un_b.s_b3 = b3;
    inAddress->sin_addr.S_un.S_un_b.s_b4 = b4;
}

void efef::socket_addr::fill(wstr presentation, const ushort port)
{
    sockaddr_in* inAddress = efef::GetIPv4(mAddress);

    inAddress->sin_family = AF_INET;
    inAddress->sin_port = htons(port);

    switch (InetPton(AF_INET, presentation, &inAddress->sin_addr))
    {
    case 0:
        efef::DebugError(L"Socket Address Constructor PtoN Error: Malformed string");
        break;
    case SOCKET_ERROR:
        efef::DebugError(L"Socket Address Constructor PtoN Error");
        break;
    }
}

bool efef::socket_addr::operator==(const socket_addr& addr) const
{
    for (uint i = 0; i < ADDR_SIZE; ++i)
        if (mAddress[i] != addr.mAddress[i])
            return false;
    return true;
}

bool efef::socket_addr::operator==(const bool& active) const
{
    for (uint i = 0; i < ADDR_SIZE; ++i)
        if (mAddress[i] != 0)
            return active;
    return !active;
}
