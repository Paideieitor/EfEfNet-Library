#include "efefFastSocket.h"

#include "efefPrivateGlobals.h"

void efef::fast_socket::connect(const socket_addr& addr)
{
}

void efef::fast_socket::accept(socket_addr& addr)
{
}

void efef::fast_socket::give_acces(const socket_addr& addr)
{

}

void efef::fast_socket::ban(const socket_addr& addr)
{
}

int efef::fast_socket::send(const byte* data, int dataLength)
{
	return 0;
}

int efef::fast_socket::receive(byte* buffer, int bufferLength, socket_addr& sender)
{
	return 0;
}

int efef::fast_socket::send_message(const byte* data, int dataLength, socket_addr& receiver) const
{
	int bytesSent = sendto(mSocket, (const char*)data, dataLength, 0, efef::GetAddress(receiver.mAddress), ADDR_SIZE);

	if (bytesSent < dataLength)
		return efef::DebugError("Fast Socket Send Message To Error");

	return bytesSent;
}

int efef::fast_socket::receive_message(const byte* buffer, int bufferLength, socket_addr& sender) const
{
	int senderLength = ADDR_SIZE;
	int bytesRead = recvfrom(mSocket, (char*)buffer, bufferLength, 0, efef::GetAddress(sender.mAddress), &senderLength);

	if (bytesRead < 0)
		return efef::DebugError("UDP Socket Recieve From Error");

	return bytesRead;
}
