#pragma once

#include "efefSocket.h"

namespace efef
{
	class udp_socket : public efef_socket
	{
    public:

        int SendTo(const byte* data, int dataLength, socket_addr& receiver);
        int SendTo(const byte* data, int dataLength, socket_addr&& receiver);
        int ReceiveFrom(byte* buffer, int bufferLength, socket_addr& sender);

    private:

        udp_socket(uint efef_socket);

        friend udp_socket CreateUDPSocket(address_family);
	};
}
