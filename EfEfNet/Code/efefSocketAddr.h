#pragma once

#include "efefGlobals.h"

namespace efef
{
    typedef char address[ADDR_SIZE];

	class socket_addr
	{
	public:

        socket_addr();

        // passing a 4-bit ip address and a port creates an AF_INET efef_socket
        socket_addr(const ulong address, const ushort port);

        // passing a 4 int ip address and a port creates an AF_INET efef_socket
        socket_addr(const int b1, const int b2, const int b3, const int b4, const ushort port);

        // passing a string presentation of the ip address and a port creates an AF_INET efef_socket
        socket_addr(wstr presentation, const ushort port);

    private:

        address mAddress;

        friend class efef_socket;
        friend class udp_socket;
        friend class tcp_socket;
        friend class fast_socket;
	};
}
