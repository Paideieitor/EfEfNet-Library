#include <Windows.h>
#include <WinUser.h>

#include "efefNet.h"
#include "efefStreams.h"

#include "Render.h"
Render* render = nullptr;

#include <iostream>
#include <string>

bool GetKey(int VKey)
{
	int pressed = GetKeyState(VKey);
	if (pressed != 0 && pressed != 1)
		return true;
	return false;
}

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

	while (true)
	{
		if (GetKey(VK_LCONTROL))
			socket1.disconnect();
		if (GetKey(VK_RCONTROL))
		{
			socket1.connect(address2);
			socket2.connect(address1);

			socket1.send(input1.get_buffer(), input1.size());
		}

		if (!GetKey(VK_SPACE))
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
						wchar_t words[64];
						render->Log(output.get_array(words, 64));

						messages[i].destroy();

						socket1.send(input1.get_buffer(), input1.size());
					}
					break;
					case efef::message_type::DISCONNECT:
						render->Log(L"Socket 1 Disconnected");
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
					wchar_t words[64];
					render->Log(output.get_array(words, 64));

					messages[i].destroy();

					socket2.send(input2.get_buffer(), input2.size());
				}
				break;
				case efef::message_type::DISCONNECT:
					render->Log(L"Socket 2 Disconnected");
					break;
				case efef::message_type::FAIL:
					break;
				}
			}
		}
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

enum class cstate
{
	DISC,
	CON
};

int main()
{
	render = new Render(true);

	if (efef::Init() == EFEF_ERROR)
	{
		return 0;
	}
	render->Start();
	
	efef::manager* const manager = efef::manager::instance();

	efef::socket_addr serveraddress(L"127.0.0.1", 6000u);
	efef::server server; 
	server.fist_port = 6005u;
	server.bind(serveraddress);

	cstate state = cstate::DISC;

	efef::socket_addr address1(L"127.0.0.1", 6001u);
	efef::fast_socket socket1 = efef::CreateFastSocket(efef::address_family::IPv4);
	socket1.bind(address1);
	socket1.connect(serveraddress);

	efef::istream input1;
	input1.push_var(9u);
	input1.push_array("Socket 1", 9u);
	socket1.send(input1.get_buffer(), input1.size());

	while (true)
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
					switch (state)
					{
					case cstate::DISC:
					{
						efef::ostream output(messages[i].data, messages[i].size);

						uint ID = output.get_var<uint>();
						uint port = output.get_var<uint>();

						efef::socket_addr newserveraddress(L"127.0.0.1", port);
						socket1.connect(newserveraddress);

						efef::istream input;
						input.push_var(124u);
						socket1.send(input.get_buffer(), input.size());

						state = cstate::CON;
					}
					break;
					case cstate::CON:
					{
						efef::ostream output(messages[i].data, messages[i].size);
						uint n = output.get_var<uint>();

						wchar_t mesg[64];
						swprintf_s(mesg, L"Client Received: %u", n);
						render->Log(mesg);

						efef::istream input;
						input.push_var(124u);
						socket1.send(input.get_buffer(), input.size());

						if (GetKey(VK_SPACE))
						{
							socket1.disconnect();
							state = cstate::DISC;
						}
					}
					break;
					}
					messages[i].destroy();
				}
				break;
				case efef::message_type::DISCONNECT:
					break;
				case efef::message_type::FAIL:
					break;
				}
			}
		}

		efef::set<const efef::server::client*> clients = server.pendant_clients();
		for (uint c = 0u; c < clients.size(); ++c)
		{
			for (uint i = 0u; i < clients[c]->messages.size(); ++i)
			{
				switch (clients[c]->messages[i].type)
				{
				case efef::MESSAGE:
				{
					efef::ostream output(clients[c]->messages[i].data, clients[c]->messages[i].size);
					uint n = output.get_var<uint>();

					wchar_t mesg[64];
					swprintf_s(mesg, L"Server Received: %u", n);
					render->Log(mesg);

					efef::istream input;
					input.push_var(7707u);
					server.send_to(clients[c], input.get_buffer(), input.size());
				}
					break;
				case efef::DISCONNECT:
					render->Log(L"HE DISCONNECTED!");
					break;
				case efef::FAIL:
					break;
				}
			}
		}

		// RENDER
		if (GetKey(VK_ESCAPE))
			break;

		for (uint i = 0u; i < manager->sockets.size(); ++i)
		{
			efef::fast_socket* socket = manager->sockets[i];

			wchar_t recvIDs[126] = L"[";
			for (uint r = 0u; r < socket->recvIDs.size(); ++r)
				if (r != 0u)
					swprintf_s(recvIDs, L"%s, %u", recvIDs, socket->recvIDs[r]);
				else
					swprintf_s(recvIDs, L"%s %u", recvIDs, socket->recvIDs[r]);
			swprintf_s(recvIDs, L"%s ]", recvIDs);

			wchar_t data[1024];
			swprintf_s(data, L"Socket %u\nmessages: %u/%u\nunaknowledged: %u/%u\nreceived IDs: %u/%u -> %s\nnext ID: %u\n%u / %u\nconnected: %u",
				i + 1u, socket->messages.size(), socket->messages.capacity(), socket->unaknowledged.size(), socket->unaknowledged.capacity(), 
				socket->recvIDs.size(), socket->recvIDs.capacity(), recvIDs, socket->nextID,
				GetTickCount() - socket->lastSentTime, socket->send_rate_time, (uint)(socket->mRemote == true));
			
			render->RenderText(0, data, fPoint(0.0f, (float)(0 + 8 * i)));
		}

		
		manager->update();
		render->Update(0.0f);
	}

	if (efef::CleanUp() == EFEF_ERROR)
	{
		return 0;
	}
	render->CleanUp();

	return 0;
}

/*
	render = new Render(true);

	if (efef::Init() == EFEF_ERROR)
	{
		return 0;
	}
	render->Start();


	FastSocketTest();
	if (false)
	{
		efef::socket_addr serveraddress(L"127.0.0.1", 6000u);
		efef::socket_addr address1(L"127.0.0.1", 6001u);

		efef::server server; server.fist_port = 6005u;
		efef::fast_socket socket1 = efef::CreateFastSocket(efef::address_family::IPv4);
		cstate state = cstate::DISC;

		server.bind(serveraddress);
		socket1.bind(address1);

		socket1.connect(serveraddress);

		efef::istream input1;
		input1.push_var(9u);
		input1.push_array("Socket 1", 9u);
		socket1.send(input1.get_buffer(), input1.size());

		while (true)
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
						switch (state)
						{
						case cstate::DISC:
						{
							efef::ostream output(messages[i].data, messages[i].size);

							uint ID = output.get_var<uint>();
							uint port = output.get_var<uint>();

							messages[i].destroy();

							efef::socket_addr newserveraddress(L"127.0.0.1", port);
							socket1.connect(newserveraddress);

							//efef::istream input;
							//input.push_var(124u);
							//socket1.send(input.get_buffer(), input.size());
						}
							break;
						case cstate::CON:
						{
							efef::ostream output(messages[i].data, messages[i].size);
							uint n = output.get_var<uint>();

							socket1.disconnect();
						}
							break;
						}
					}
					break;
					case efef::message_type::DISCONNECT:
						break;
					case efef::message_type::FAIL:
						break;
					}
				}
			}

			efef::set<const efef::server::client*> clients = server.pendant_clients();
			for (uint c = 0u; c < clients.size(); ++c)
			{
				for (uint i = 0u; i < clients[c]->messages.size(); ++i)
				{
					efef::ostream output(clients[c]->messages[i].data, clients[c]->messages[i].size);
					uint n = output.get_var<uint>();

					//efef::istream input;
					//socket1.send(input.get_buffer(), input.size());
					//server.send_to(clients[c], input.get_buffer(), input.size());
				}
			}

			//efef::manager::instance()->update();
			//render->Update(0.0f);
		}

		//PrintClientList(server);
	}

	if (efef::CleanUp() == EFEF_ERROR)
	{
		return 0;
	}
	render->CleanUp();

	return 0;
*/