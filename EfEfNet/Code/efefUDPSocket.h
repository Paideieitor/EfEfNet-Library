#pragma once

#include "efefSocket.h"

namespace efef
{
	class udp_socket : public efef_socket
	{
    public:

        int send_to(const byte* data, int dataLength, socket_addr& receiver);
        int send_to(const byte* data, int dataLength, socket_addr&& receiver);
        int receive_from(byte* buffer, int bufferLength, socket_addr& sender);

    private:

        udp_socket(uint efef_socket);

        friend udp_socket CreateUDPSocket(address_family);
	};
}
