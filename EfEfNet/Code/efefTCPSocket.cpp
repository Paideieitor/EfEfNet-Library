#include "efefTCPSocket.h"

#include "efefPrivateGlobals.h"

#include "efefSocketAddr.h"

efef::tcp_socket::tcp_socket(uint socket) : efef_socket(socket) {}

int efef::tcp_socket::connect(socket_addr& address) const
{
    int error = ws2connect(mSocket, efef::GetAddress(address.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("TCP Socket Connect Error");

    return EFEF_NO_ERROR;
}

int efef::tcp_socket::connect(socket_addr&& address) const
{
    int error = ws2connect(mSocket, efef::GetAddress(address.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("TCP Socket Connect Error");

    return EFEF_NO_ERROR;
}

int efef::tcp_socket::listen(int backLog) const
{
    int error = ws2listen(mSocket, backLog);
    
    if (error < 0)
        return efef::DebugError("TCP Socket Listen Error");

    return EFEF_NO_ERROR;
}

efef::tcp_socket efef::tcp_socket::accept(socket_addr& fromAddress) const
{
    int fromLength = ADDR_SIZE;
    SOCKET newSocket = ws2accept(mSocket, efef::GetAddress(fromAddress.mAddress), &fromLength);

    if (newSocket != INVALID_SOCKET)
        return tcp_socket((uint)newSocket);

    return efef::DebugError("TCP Socket Accept Error");
}

int efef::tcp_socket::send(const byte* data, int dataLength) const
{
    int bytesSent = ws2send(mSocket, (const char*)data, dataLength, 0);

    if (bytesSent < dataLength)
        return efef::DebugError("TCP Socket Send Error");

    return bytesSent;
}

int efef::tcp_socket::receive(byte* buffer, int bufferLength) const
{
    int bytesRecv = recv(mSocket, (char*)buffer, bufferLength, 0);

    if (bytesRecv < 0)
        return efef::DebugError("TCP Socket Receive Error");

    return bytesRecv;
}
