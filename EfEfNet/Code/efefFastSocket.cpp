#include "efefFastSocket.h"

#include "efefPrivateGlobals.h"
#include "efefInputStream.h"

efef::fast_socket::fast_socket(uint socket) : efef_socket(socket), next_id(0u) 
{
    efef::memory_set(recv_ids, 0, RECVID_SIZE);
}

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

void efef::fast_socket::send(const byte* data, int dataLength)
{
    if (to_send.size() == 0u)
        to_send.push_var("\\");
    to_send.push_var(next_id++);
    to_send.push_array(data, dataLength);
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

void efef::fast_socket::update()
{

}

void efef::fast_socket::send_message()
{
    int bytesSent = sendto(mSocket, (const char*)to_send.get_buffer(), to_send.size(), 0, efef::GetAddress(mAddress.mAddress), ADDR_SIZE);

    if (bytesSent < to_send.size())
        efef::DebugError("Fast Socket Send Message To Error");

    to_send.clear();
}
