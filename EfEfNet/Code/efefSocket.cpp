#include "efefSocket.h"

#include "efefPrivateGlobals.h"
#include "efefDebug.h"

#include "efefSocketAddr.h"

efef::efef_socket::efef_socket(uint efef_socket) : mSocket(efef_socket), mBlocking(false) {}

efef::efef_socket::~efef_socket()
{
    closesocket((SOCKET)mSocket);
}

int efef::efef_socket::bind(socket_addr& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Socket Bind Error");

    return EFEF_NO_ERROR;
}

int efef::efef_socket::bind(socket_addr&& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Socket Bind Error");

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
        return efef::DebugError("Socket SetBlocking Error");

    mBlocking = enable;
    return EFEF_NO_ERROR;
}
