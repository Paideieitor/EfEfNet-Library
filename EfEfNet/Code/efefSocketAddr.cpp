#include "efefSocketAddr.h"

#include "efefPrivateGlobals.h"
#include "efefDebug.h"

efef::socket_addr::socket_addr()
{
    memset(mAddress, 0, ADDR_SIZE);
}

efef::socket_addr::socket_addr(const ulong address, const ushort port)
{
    sockaddr_in* inAddress = efef::GetIPv4(mAddress);

    inAddress->sin_family = AF_INET;
    inAddress->sin_addr.S_un.S_addr = htonl(address);
    inAddress->sin_port = htons(port);
}

efef::socket_addr::socket_addr(const int b1, const int b2, const int b3, const int b4, const ushort port)
{
    sockaddr_in* inAddress = efef::GetIPv4(mAddress);

    inAddress->sin_family = AF_INET;
    inAddress->sin_port = htons(port);

    inAddress->sin_addr.S_un.S_un_b.s_b1 = b1;
    inAddress->sin_addr.S_un.S_un_b.s_b2 = b2;
    inAddress->sin_addr.S_un.S_un_b.s_b3 = b3;
    inAddress->sin_addr.S_un.S_un_b.s_b4 = b4;
}

efef::socket_addr::socket_addr(wstr presentation, const ushort port)
{
    sockaddr_in* inAddress = efef::GetIPv4(mAddress);

    inAddress->sin_family = AF_INET;
    inAddress->sin_port = htons(port);

    switch (InetPton(AF_INET, presentation, &inAddress->sin_addr))
    {
    case 0:
        efef::DebugError("Socket Address Constructor PtoN Error: Malformed string");
        break;
    case SOCKET_ERROR:
        efef::DebugError("Socket Address Constructor PtoN Error");
        break;
    }
}
