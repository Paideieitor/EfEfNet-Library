#pragma once

#include "efefTypes.h"

namespace efef
{
	class socket_addr;

	class efef_socket
	{
	public:

		virtual ~efef_socket();

		int Bind(socket_addr&);
		int Bind(socket_addr&&);

		bool IsBlocking();
		int SetBlocking(bool enable);

	protected:

		efef_socket(uint efef_socket);

		uint mSocket;
		bool blocking;
	};
}
