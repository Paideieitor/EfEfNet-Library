#include "efefNet.h"

#include <iostream>

void PrintMessage(byte* message, uint length)
{
	for (uint i = 0; i < length; ++i)
		std::cout << message[i];
	std::cout << std::endl;
}

int main()
{
	if (efef::Init() == EFEF_ERROR)
	{
		std::cout << "Init Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	efef::udp_socket sender = efef::CreateUDPSocket(efef::IPv4);
	efef::socket_addr senderAddr = efef::socket_addr(L"127.0.0.1", 6000);
	sender.Bind(senderAddr);

	efef::udp_socket receiver = efef::CreateUDPSocket(efef::IPv4);
	efef::socket_addr receiverAddr = efef::socket_addr(L"127.0.0.1", 6001);
	receiver.Bind(receiverAddr);

	const char* data = "Hello World!";
	int bytesSent = sender.SendTo((const byte*)data, 12, receiverAddr);
	if (bytesSent == EFEF_ERROR)
	{
		std::cout << "Send Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	byte buffer[64];
	memset(buffer, 0, 64);

	efef::socket_addr newAddr;
	int bytesReceived = receiver.ReceiveFrom(buffer, 64, newAddr);
	if (bytesReceived == EFEF_ERROR)
	{
		std::cout << "Receive Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	PrintMessage(buffer, 64);

	if (efef::CleanUp() == EFEF_ERROR)
	{
		std::cout << "Clean Up Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	return 0;
}