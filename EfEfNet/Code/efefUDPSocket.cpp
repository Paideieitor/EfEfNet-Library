#include "efefUDPSocket.h"

#include "efefGlobal.h"
#include "efefDebug.h"

#include "efefSocketAddr.h"

efef::udp_socket::udp_socket(uint efef_socket) : efef_socket(efef_socket) {}

int efef::udp_socket::SendTo(const byte* data, int dataLength, socket_addr& receiver)
{
    int bytesSent = sendto(mSocket, (const char*)data, dataLength, 0, efef::GetAddress(receiver.mAddress), ADDR_SIZE);

    if (bytesSent < dataLength)
        return efef::DebugError("UDP Socket Send To Error");

    return bytesSent;
}

int efef::udp_socket::SendTo(const byte* data, int dataLength, socket_addr&& receiver)
{
    int bytesSent = sendto(mSocket, (const char*)data, dataLength, 0, efef::GetAddress(receiver.mAddress), ADDR_SIZE);

    if (bytesSent < dataLength)
        return efef::DebugError("UDP Socket Send To Error");

    return bytesSent;
}

int efef::udp_socket::ReceiveFrom(byte* buffer, int bufferLength, socket_addr& sender)
{
    int senderLength = ADDR_SIZE;
    int bytesRead = recvfrom(mSocket, (char*)buffer, bufferLength, 0, efef::GetAddress(sender.mAddress), &senderLength);

    if (bytesRead < 0)
        return efef::DebugError("UDP Socket Recieve From Error");

    return bytesRead;
}
