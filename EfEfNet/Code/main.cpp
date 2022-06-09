#include "efefNet.h"
#include "efefStreams.h"

#include <iostream>
#include <string>

void FastSocketTest()
{
	efef::socket_addr address1(L"127.0.0.1", 6000u);
	efef::socket_addr address2(L"127.0.0.1", 6001u);

	efef::fast_socket socket1 = efef::CreateFastSocket(efef::address_family::IPv4);
	efef::fast_socket socket2 = efef::CreateFastSocket(efef::address_family::IPv4);

	socket1.bind(address1);
	socket2.bind(address2);

	socket1.connect(address2);
	socket2.connect(address1);

	efef::istream input1;
	input1.push_array("Socket 1", 9u);

	efef::istream input2;
	input2.push_array("Socket 2", 9u);

	socket1.send(input1.get_buffer(), input1.size());

	for (uint frame = 0u; frame < 100u; ++frame)
	{
		if (socket2.poll(efef::select_mode::RECEIVE))
		{
			efef::set<efef::message> messages = socket2.receive();
			for (uint i = 0u; i < messages.size(); ++i)
			{
				switch (messages[i].type)
				{
				case efef::message_type::MESSAGE:
				{
					std::cout << "Socket 2 Message Received : ";
					efef::ostream output(messages[i].data, messages[i].size);
					char words[64];
					std::cout << output.get_array(words, 64) << "\n";

					delete[] messages[i].data;

					//socket2.send(input2.get_buffer(), input2.size());

					socket2.disconnect();
				}
				break;
				case efef::message_type::DISCONNECT:
					std::cout << "Socket 2 Disconnected\n";
					break;
				case efef::message_type::FAIL:
					break;
				}
			}
		}

		if (socket1.poll(efef::select_mode::RECEIVE))
		{
			efef::set<efef::message> messages = socket1.receive();
			for (uint i = 0u; i < messages.size(); ++i)
			{
				switch (messages[i].type)
				{
				case efef::message_type::MESSAGE:
				{
					std::cout << "Socket 1 Message Received : ";
					efef::ostream output(messages[i].data, messages[i].size);
					char words[64];
					std::cout << output.get_array(words, 64) << "\n";

					delete[] messages[i].data;

					socket1.send(input1.get_buffer(), input1.size());
				}
				break;
				case efef::message_type::DISCONNECT:
					std::cout << "Socket 1 Disconnected\n";
					break;
				case efef::message_type::FAIL:
					break;
				}
			}
		}

		efef::manager::instance()->update();
	}
}

void PrintClientList(const efef::server& server)
{
	std::cout << "Client List\n";
	for (uint i = 0u; i < server.clients.size(); ++i)
	{
		efef::server::client& client = server.clients[i];

		std::string state = " ";
		switch (client.state)
		{
		case efef::server::client_state::CONNECTED:
			state = "CONNECTED";
			break;
		case efef::server::client_state::DISCONNECTED:
			state = "DISCONNECTED";
			break;
		}
		std::cout << "[ " << client.name << ", " << client.ID << ", " << state << " ]\n";
	}
	std::cout << '\n';
}

int main()
{
	if (efef::Init() == EFEF_ERROR)
	{
		std::cout << "Init Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	{
		efef::socket_addr serveraddress(L"127.0.0.1", 6000u);
		efef::socket_addr address1(L"127.0.0.1", 6001u);
		efef::socket_addr address2(L"127.0.0.1", 6002u);
		efef::socket_addr address3(L"127.0.0.1", 6003u);
		efef::socket_addr address4(L"127.0.0.1", 6004u);

		efef::server server; server.fist_port = 6005u;
		efef::fast_socket socket1 = efef::CreateFastSocket(efef::address_family::IPv4);
		efef::fast_socket socket2 = efef::CreateFastSocket(efef::address_family::IPv4);
		efef::fast_socket socket3 = efef::CreateFastSocket(efef::address_family::IPv4);
		efef::fast_socket socket4 = efef::CreateFastSocket(efef::address_family::IPv4);

		server.bind(serveraddress);
		socket1.bind(address1);
		socket2.bind(address2);
		socket3.bind(address3);
		socket4.bind(address4);

		socket1.connect(serveraddress);
		socket2.connect(serveraddress);
		socket3.connect(serveraddress);
		socket4.connect(serveraddress);

		efef::istream input1;
		input1.push_var(9u);
		input1.push_array("Socket 1", 9u);
		socket1.send(input1.get_buffer(), input1.size());

		efef::istream input2;
		input2.push_var(9u);
		input2.push_array("Socket 2", 9u);
		socket2.send(input2.get_buffer(), input2.size());

		efef::istream input3;
		input3.push_var(9u);
		input3.push_array("Socket 3", 9u);
		socket3.send(input3.get_buffer(), input3.size());

		efef::istream input4;
		input4.push_var(9u);
		input4.push_array("Socket 4", 9u);
		socket4.send(input4.get_buffer(), input4.size());

		for (uint frame = 0u; frame < 100u; ++frame)
		{
			if (socket1.poll(efef::select_mode::RECEIVE))
			{
				efef::set<efef::message> messages = socket1.receive();
				for (uint i = 0u; i < messages.size(); ++i)
				{
					switch (messages[i].type)
					{
					case efef::message_type::MESSAGE:
					{
						efef::ostream output(messages[i].data, messages[i].size);

						uint ID = output.get_var<uint>();
						uint port = output.get_var<uint>();

						delete[] messages[i].data;

						efef::socket_addr newserveraddress(L"127.0.0.1", port);
						socket1.connect(newserveraddress);
					}
					break;
					case efef::message_type::DISCONNECT:
						std::cout << "Socket 1 Disconnected\n";
						break;
					case efef::message_type::FAIL:
						break;
					}
				}
			}

			if (socket2.poll(efef::select_mode::RECEIVE))
			{
				efef::set<efef::message> messages = socket2.receive();
				for (uint i = 0u; i < messages.size(); ++i)
				{
					switch (messages[i].type)
					{
					case efef::message_type::MESSAGE:
					{
						efef::ostream output(messages[i].data, messages[i].size);

						uint ID = output.get_var<uint>();
						uint port = output.get_var<uint>();

						delete[] messages[i].data;

						efef::socket_addr newserveraddress(L"127.0.0.1", port);
						socket2.connect(newserveraddress);

						socket2.disconnect();
					}
					break;
					case efef::message_type::DISCONNECT:
						std::cout << "Socket 2 Disconnected\n";
						break;
					case efef::message_type::FAIL:
						break;
					}
				}
			}

			if (socket3.poll(efef::select_mode::RECEIVE))
			{
				efef::set<efef::message> messages = socket3.receive();
				for (uint i = 0u; i < messages.size(); ++i)
				{
					switch (messages[i].type)
					{
					case efef::message_type::MESSAGE:
					{
						efef::ostream output(messages[i].data, messages[i].size);

						uint ID = output.get_var<uint>();
						uint port = output.get_var<uint>();

						delete[] messages[i].data;

						efef::socket_addr newserveraddress(L"127.0.0.1", port);
						socket3.connect(newserveraddress);

						socket3.send(input3.get_buffer(), input3.size());
					}
					break;
					case efef::message_type::DISCONNECT:
						std::cout << "Socket 3 Disconnected\n";
						break;
					case efef::message_type::FAIL:
						break;
					}
				}
			}

			if (socket4.poll(efef::select_mode::RECEIVE))
			{
				efef::set<efef::message> messages = socket4.receive();
				for (uint i = 0u; i < messages.size(); ++i)
				{
					switch (messages[i].type)
					{
					case efef::message_type::MESSAGE:
					{
						efef::ostream output(messages[i].data, messages[i].size);

						uint ID = output.get_var<uint>();
						uint port = output.get_var<uint>();

						delete[] messages[i].data;

						efef::socket_addr newserveraddress(L"127.0.0.1", port);
						socket4.connect(newserveraddress);

						socket4.send(input4.get_buffer(), input4.size());
					}
					break;
					case efef::message_type::DISCONNECT:
						std::cout << "Socket 4 Disconnected\n";
						break;
					case efef::message_type::FAIL:
						break;
					}
				}
			}

			efef::manager::instance()->update();
		}

		PrintClientList(server);
	}

	if (efef::CleanUp() == EFEF_ERROR)
	{
		std::cout << "Clean Up Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	return 0;
}