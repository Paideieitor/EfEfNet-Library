#include "efefNet.h"
#include "efefStreams.h"

#include <iostream>

int main()
{
	if (efef::Init() == EFEF_ERROR)
	{
		std::cout << "Init Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	efef::udp_socket sender = efef::CreateUDPSocket(efef::IPv4);
	efef::socket_addr senderAddr = efef::socket_addr(L"127.0.0.1", 6000);
	sender.bind(senderAddr);

	efef::udp_socket receiver = efef::CreateUDPSocket(efef::IPv4);
	efef::socket_addr receiverAddr = efef::socket_addr(L"127.0.0.1", 6001);
	receiver.bind(receiverAddr);

	efef::istream is;
	is.push_var(13);
	is.push_array("Hello World!", 13);

	int bytesSent = sender.send_to(is.get_buffer(), is.size(), receiverAddr);
	if (bytesSent == EFEF_ERROR)
	{
		std::cout << "Send Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	byte buffer[64];
	memset(buffer, 0, 64);

	efef::socket_addr newAddr;
	int bytesReceived = receiver.receive_from(buffer, 64, newAddr);
	if (bytesReceived == EFEF_ERROR)
	{
		std::cout << "Receive Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	efef::ostream os(buffer, bytesReceived);

	uint sSize = os.get_var<uint>();
	char* sentence = new char[sSize];
	std::cout << os.get_array(sentence, sSize);
	delete[] sentence;

	if (efef::CleanUp() == EFEF_ERROR)
	{
		std::cout << "Clean Up Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	return 0;
}