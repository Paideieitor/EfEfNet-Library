#pragma once

#include "efefFastSocket.h"

#define SERVER_SIZE 100u

namespace efef
{
	class server
	{
	public:

		~server();

		enum client_state
		{
			DISCONNECTED,
			CONNECTED
		};
		struct client
		{
			char* name;
			uint name_size;
			char* password;
			uint pw_size;

			socket_addr address;
			uint ID;
			client_state state;

			set<message> messages;
		};

		void bind(socket_addr& address);
		void bind(socket_addr&& address);

		int send_to(uint ID, byte* data, uint dataLength);
		int send_to(const client* client, byte* data, uint dataLength);

		set<const client*> pendant_clients();

		uint fist_port = 6000u;
		set<client> clients;

	private:

		void update();

		fast_socket listen_socket;
		fast_socket sockets[SERVER_SIZE];

		set<client*> delete_messages;

		friend class manager;
	};
}