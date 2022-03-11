#include "efefTCPSocket.h"

#include "efefPrivateGlobals.h"
#include "efefDebug.h"

#include "efefSocketAddr.h"

efef::tcp_socket::tcp_socket(uint efef_socket) : efef_socket(efef_socket) {}

int efef::tcp_socket::connect(socket_addr& address)
{
    int error = ws2connect(mSocket, efef::GetAddress(address.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("TCP Socket Connect Error");

    return EFEF_NO_ERROR;
}

int efef::tcp_socket::connect(socket_addr&& address)
{
    int error = ws2connect(mSocket, efef::GetAddress(address.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("TCP Socket Connect Error");

    return EFEF_NO_ERROR;
}

int efef::tcp_socket::listen(int backLog)
{
    int error = ws2listen(mSocket, backLog);
    
    if (error < 0)
        return efef::DebugError("TCP Socket Listen Error");

    return EFEF_NO_ERROR;
}

efef::tcp_socket efef::tcp_socket::accept(socket_addr& fromAddress)
{
    int fromLength = ADDR_SIZE;
    SOCKET newSocket = ws2accept(mSocket, efef::GetAddress(fromAddress.mAddress), &fromLength);

    if (newSocket != INVALID_SOCKET)
        return tcp_socket(newSocket);

    return efef::DebugError("TCP Socket Accept Error");
}

int efef::tcp_socket::send(const byte* data, int dataLength)
{
    int bytesSent = ws2send(mSocket, (const char*)data, dataLength, 0);

    if (bytesSent < dataLength)
        return efef::DebugError("TCP Socket Send Error");

    return bytesSent;
}

int efef::tcp_socket::receive(byte* buffer, int bufferLength)
{
    int bytesRecv = recv(mSocket, (char*)buffer, bufferLength, 0);

    if (bytesRecv < 0)
        return efef::DebugError("TCP Socket Receive Error");

    return bytesRecv;
}
