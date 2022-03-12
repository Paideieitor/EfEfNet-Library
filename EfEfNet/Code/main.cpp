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

	efef::udp_socket receiver = efef::CreateUDPSocket(efef::IPv4);
	efef::socket_addr receiverAddr = efef::socket_addr(L"127.0.0.1", 6000);
	receiver.bind(receiverAddr);

	{	// SENDING //
		efef::udp_socket sender = efef::CreateUDPSocket(efef::IPv4);
		efef::socket_addr senderAddr = efef::socket_addr(L"127.0.0.1", 6001);
		if (sender.bind(senderAddr) == EFEF_ERROR)
		{
			std::cout << "Bind sender 1\n";
			return 0;
		}
		efef::udp_socket sender2 = efef::CreateUDPSocket(efef::IPv4);
		efef::socket_addr sender2Addr = efef::socket_addr(L"127.0.0.1", 6002);
		if (sender2.bind(sender2Addr) == EFEF_ERROR)
		{
			std::cout << "Bind sender 2\n";
			return 0;
		}
		efef::udp_socket sender3 = efef::CreateUDPSocket(efef::IPv4);
		efef::socket_addr sender3Addr = efef::socket_addr(L"127.0.0.1", 6003);
		if (sender3.bind(sender3Addr) == EFEF_ERROR)
		{
			std::cout << "Bind sender 3\n";
			return 0;
		}

		efef::udp_set set;
		set.add(&sender);
		set.add(&sender2);
		set.add(&sender3);

		for (uint i = 0; i < set.size(); ++i)
		{
			efef::istream is;
			is.push_var(13);
			is.push_array("Hello World!", 13);
			is.push_var(i);

			int bytesSent = set[i]->send_to(is.get_buffer(), is.size(), receiverAddr);
			if (bytesSent == EFEF_ERROR)
			{
				std::cout << "Send 1 Error: " << efef::GetLastError() << std::endl;
				return 0;
			}
			efef::ostream os(is.get_buffer(), is.size());
			const char* str = os.dynm_get_array<char>(os.get_var<uint>());
			std::cout << "Sent: " << str << ' ' << os.get_var<uint>() << '\n';
			delete[] str;
		}
	}

	{	// RECEIVING
		receiver.set_blocking(false);

		while (efef::Poll(&receiver, efef::RECEIVE, 0l) == 1)
		{
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
			std::cout << os.get_array(sentence, sSize) << ' ';
			std::cout << os.get_var<uint>() << '\n';
			delete[] sentence;
		}
	}

	if (efef::CleanUp() == EFEF_ERROR)
	{
		std::cout << "Clean Up Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	std::cout << "End" << std::endl;
	return 0;
}