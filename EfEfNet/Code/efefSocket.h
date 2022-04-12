#pragma once

#include "efefGlobals.h"

namespace efef
{
	class socket_addr;

	class efef_socket
	{
	public:

		virtual ~efef_socket();

		virtual int bind(socket_addr&);
		virtual int bind(socket_addr&&);

		bool is_blocking();
		int set_blocking(bool enable);

	protected:

		efef_socket(uint socket);

		uint mSocket;
		bool mBlocking;

		friend class priv_util;
	};
}
