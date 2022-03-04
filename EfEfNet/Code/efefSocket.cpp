#include "efefSocket.h"

#include "efefGlobal.h"
#include "efefDebug.h"

#include "efefSocketAddr.h"

efef::efef_socket::efef_socket(uint efef_socket) : mSocket(efef_socket), blocking(false) {}

efef::efef_socket::~efef_socket()
{
    closesocket((SOCKET)mSocket);
}

int efef::efef_socket::Bind(socket_addr& addr)
{
    int error = bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Socket Bind Error");

    return EFEF_NO_ERROR;
}

int efef::efef_socket::Bind(socket_addr&& addr)
{
    int error = bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Socket Bind Error");

    return EFEF_NO_ERROR;
}

bool efef::efef_socket::IsBlocking()
{
    return blocking;
}

int efef::efef_socket::SetBlocking(bool enable)
{
    u_long arg = enable ? 0 : 1;

    int error = ioctlsocket((SOCKET)mSocket, FIONBIO, &arg);
    if (error < 0)
        return efef::DebugError("Socket SetBlocking Error");

    return EFEF_NO_ERROR;
}
