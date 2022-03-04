#pragma once

#include "efefSocket.h"

namespace efef
{
    class tcp_socket : public efef_socket
    {
    public:

        int Connect(socket_addr& address);
        int Connect(socket_addr&& address);
        int Listen(int backLog = 0x7fffffff);
        tcp_socket Accept(socket_addr& fromAddress);

        int Send(const byte* data, int dataLength);
        int Receive(byte* buffer, int bufferLength);

    private:

        tcp_socket(uint efef_socket);

        friend tcp_socket CreateTCPSocket(address_family);
    };
}

