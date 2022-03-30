#include "efefUDPSocket.h"

#include "efefPrivateGlobals.h"

#include "efefSocketAddr.h"

efef::udp_socket::udp_socket(uint socket) : efef_socket(socket) {}

int efef::udp_socket::send_to(const byte* data, int dataLength, socket_addr& receiver) const
{
    int bytesSent = sendto(mSocket, (const char*)data, dataLength, 0, efef::GetAddress(receiver.mAddress), ADDR_SIZE);

    if (bytesSent < dataLength)
        return efef::DebugError("UDP Socket Send To Error");

    return bytesSent;
}

int efef::udp_socket::send_to(const byte* data, int dataLength, socket_addr&& receiver) const
{
    int bytesSent = sendto(mSocket, (const char*)data, dataLength, 0, efef::GetAddress(receiver.mAddress), ADDR_SIZE);

    if (bytesSent < dataLength)
        return efef::DebugError("UDP Socket Send To Error");

    return bytesSent;
}

int efef::udp_socket::receive_from(byte* buffer, int bufferLength, socket_addr& sender) const
{
    int senderLength = ADDR_SIZE;
    int bytesRead = recvfrom(mSocket, (char*)buffer, bufferLength, 0, efef::GetAddress(sender.mAddress), &senderLength);

    if (bytesRead < 0)
        return efef::DebugError("UDP Socket Recieve From Error");

    return bytesRead;
}
