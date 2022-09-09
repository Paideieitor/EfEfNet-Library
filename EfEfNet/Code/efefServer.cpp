#include "efefServer.h"

#include "efefGeneral.h"
#include "efefManager.h"
#include "efefDebug.h"
#include "efefStreams.h"

efef::server::server() : listen_socket(efef::CreateFastSocket(IPv4))
{
	memory_set(sockets, 0u, SERVER_SIZE * sizeof(fast_socket*));

	efef::manager::instance()->servers.add(this);
}

efef::server::~server()
{
	for (uint i = 0u; i < SERVER_SIZE; ++i)
		delete[] sockets[i];

	for (uint i = 0u; i < clients.size(); ++i)
		delete[] clients[i].name;
	clients.dontDelete = true;

	efef::manager* const manager = efef::manager::instance();
	uint index = 0u;
	for (uint i = 0u; i < manager->servers.size(); ++i)
		if (manager->servers[i] == this)
		{
			index = i;
			break;
		}
	efef::manager::instance()->servers.erase(index);
}

void efef::server::bind(socket_addr& address)
{
	listen_socket.bind(address);
}

void efef::server::bind(socket_addr&& address)
{
	listen_socket.bind(address);
}

int efef::server::send_to(uint ID, const byte* data, uint dataLength)
{
	sockets[ID]->send(data, dataLength);

	return EFEF_NO_ERROR;
}

int efef::server::send_to(const client* const client, const byte* data, uint dataLength)
{
	sockets[client->ID]->send(data, dataLength);

	return EFEF_NO_ERROR;
}

efef::set<const efef::server::client*>& efef::server::pendant_clients()
{
	set<const client*> output;
	output.dontDelete = true;
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
			delete_messages[i]->messages[i].destroy();

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

				ostream outStream(messages[i].data, messages[i].size);
				// Name of the client
				cl.name_size = outStream.get_var<uint>();
				cl.name = outStream.dynm_get_array<char>(cl.name_size);

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
					efef::DebugError(L"Server Acces Denied: client already logged in");
					continue;
				}
				if (!client)
				{
					clients.add(cl);
					client = &clients[clients.size() - 1u];
				}

				client->state = CONNECTED;
				client->address = messages[i].sender;

				fast_socket* socket = efef::CreateDynmcFastSocket(IPv4);

				for (uint j = 0u; j < SERVER_SIZE; ++j)
					if (!sockets[j])
					{
						sockets[j] = socket;
						client->ID = j;
						break;
					}

				socket_addr bindAddr(L"127.0.0.1", fist_port + client->ID);
				socket->bind(bindAddr);
				socket->connect(messages[i].sender);

				istream inStream;
				inStream.push_var<uint>(client->ID);
				inStream.push_var<uint>(fist_port + client->ID);

				listen_socket.connect(messages[i].sender);
				listen_socket.send(inStream.get_buffer(), inStream.size());
				listen_socket.force_send();
				listen_socket.unilateral_disconnect();
			}

			messages[i].destroy();
		}
	}

	for (uint i = 0u; i < clients.size(); ++i)
		if (clients[i].state == CONNECTED)
			if (sockets[clients[i].ID]->poll(RECEIVE))
			{
				set<message> messages = sockets[clients[i].ID]->receive();
				clients[i].messages.swap(messages);

				for (uint j = 0u; j < clients[i].messages.size(); ++j)
					if (clients[i].messages[j].type == DISCONNECT)
					{
						clients[i].state = DISCONNECTED;
						clients[i].address.fill(0u);

						delete[] sockets[clients[i].ID];
						sockets[clients[i].ID] = nullptr;
					}
			}
}
