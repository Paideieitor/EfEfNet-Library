#include "efefServer.h"

#include "efefGeneral.h"
#include "efefDebug.h"
#include "efefStreams.h"

efef::server::~server()
{
	for (uint i = 0u; i < clients.size(); ++i)
	{
		delete[] clients[i].name;
		delete[] clients[i].password;
	}
}

void efef::server::bind(socket_addr& address)
{
	listen_socket.bind(address);
}

void efef::server::bind(socket_addr&& address)
{
	listen_socket.bind(address);
}

int efef::server::send_to(uint ID, byte* data, uint dataLength)
{
	sockets[ID].send(data, dataLength);

	return EFEF_NO_ERROR;
}

int efef::server::send_to(const client* const client, byte* data, uint dataLength)
{
	sockets[client->ID].send(data, dataLength);

	return EFEF_NO_ERROR;
}

efef::set<const efef::server::client*> efef::server::pendant_clients()
{
	set<const client*> output;

	for (uint i = 0u; i < clients.size(); ++i)
		if (clients[i].messages.size() > 0u)
		{
			output.add(clients.get_ptr(i));
			delete_messages.add(clients.get_ptr(i));
		}

	return output;
}

void efef::server::update()
{
	for (uint i = 0u; i < delete_messages.size(); ++i)
	{
		for (uint j = 0u; j < delete_messages[i]->messages.size(); ++j)
			delete[] delete_messages[i]->messages[i].data;

		delete_messages[i]->messages.clear();
	}

	if (listen_socket.poll(RECEIVE))
	{
		set<message> messages = listen_socket.receive();
		for (uint i = 0u; i < messages.size(); ++i)
		{
			if (messages[i].type == MESSAGE)
			{
				client cl;

				ostream outStream(messages[i].data, messages[i].size, true);
				// Server Introduction message type -> register, login, etc.
				uint type = outStream.get_var<uint>();
				// Name of the client
				cl.name_size = outStream.get_var<uint>();
				cl.name = outStream.dynm_get_array<char>(cl.name_size);
				// Password of the client
				cl.pw_size = outStream.get_var<uint>();
				cl.password = outStream.dynm_get_array<char>(cl.pw_size);

				client* client = nullptr;
				bool error = false;
				for (uint j = 0u; j < clients.size(); ++j)
					if (clients[j].address == messages[i].sender)
					{
						error = true;
						break;
					}
					else if (clients[j].name_size == cl.name_size && efef::compare_str(clients[j].name, cl.name, cl.name_size))
					{
						client = &clients[j];
						break;
					}

				if (error)
				{
					efef::DebugError("Server Acces Denied: client already logged in");
					continue;
				}
				if (!client)
				{
					clients.add(cl);
					client = &clients[clients.size() - 1u];
				}

				client->state = CONNECTED;
				client->address = messages[i].sender;

				fast_socket socket = efef::CreateFastSocket(IPv4);

				for (uint j = 0u; j < SERVER_SIZE; ++j)
					if (sockets[j] == false)
					{
						sockets[j] = socket;
						client->ID = j;
					}

				socket_addr bindAddr(L"127.0.0.1", fist_port + client->ID);
				socket.bind(bindAddr);
				socket.connect(messages[i].sender);

				istream inStream;
				inStream.push_var<uint>(client->ID);

				listen_socket.connect(messages[i].sender);
				listen_socket.send(inStream.get_buffer(), inStream.size());
				listen_socket.force_send();
			}

			messages[i].destroy();
		}
	}

	for (uint i = 0u; i < clients.size(); ++i)
		if (sockets[clients[i].ID].poll(RECEIVE))
		{
			clients[i].messages = sockets[clients[i].ID].receive();

			for (uint j = 0u; j < clients[i].messages.size(); ++j)
				if (clients[i].messages[j].type == DISCONNECT)
				{
					clients[i].state = DISCONNECTED;

					byte sck[sizeof(fast_socket)];
					memory_set(sck, 0u, sizeof(fast_socket));
					sockets[clients[i].ID] = *reinterpret_cast<fast_socket*>(sck);
				}
		}
}
