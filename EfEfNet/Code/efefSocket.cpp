#include "efefSocket.h"

#include "efefPrivateGlobals.h"

#include "efefSocketAddr.h"

efef::efef_socket::efef_socket(uint socket) : mSocket(socket), mBlocking(true) {}

efef::efef_socket::~efef_socket()
{
    closesocket((SOCKET)mSocket);
}

int efef::efef_socket::bind(socket_addr& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError(L"Socket Bind Error");

    return EFEF_NO_ERROR;
}

int efef::efef_socket::bind(socket_addr&& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError(L"Socket Bind Error");

    return EFEF_NO_ERROR;
}

bool efef::efef_socket::is_blocking()
{
    return mBlocking;
}

int efef::efef_socket::set_blocking(bool enable)
{
    u_long arg = enable ? 0 : 1;

    int error = ioctlsocket((SOCKET)mSocket, FIONBIO, &arg);
    if (error < 0)
        return efef::DebugError(L"Socket SetBlocking Error");

    mBlocking = enable;
    return EFEF_NO_ERROR;
}
