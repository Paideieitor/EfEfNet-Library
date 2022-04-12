#include "efefFastSocket.h"

#include "efefPrivateGlobals.h"

int efef::fast_socket::bind(socket_addr& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Fast Socket Bind Error");

    mAddress = addr;
    return EFEF_NO_ERROR;
}

int efef::fast_socket::bind(socket_addr&& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Fast Socket Bind Error");

    mAddress = addr;
    return EFEF_NO_ERROR;
}

void efef::fast_socket::add_to_list(const socket_addr& address)
{
    for (uint i = 0; i < recvList.size(); ++i)
        if (address == recvList[i])
            return;

    recvList.add(address);
}

void efef::fast_socket::erase_from_list(const socket_addr& address)
{
    uint index = -1;
    for (uint i = 0; i < recvList.size(); ++i)
        if (address == recvList[i])
            index = i;
    if (index < 0)
        return;

    recvList.erase(index);
}

void efef::fast_socket::erase_from_list(uint index)
{
    if (index < 0 || index >= recvList.size())
        return;

    recvList.erase(index);
}

void efef::fast_socket::erase_list()
{
    recvList.clear();
}
