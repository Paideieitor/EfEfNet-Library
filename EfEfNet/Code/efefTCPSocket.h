#pragma once

#include "efefSocket.h"

namespace efef
{
    class tcp_socket : public efef_socket
    {
    public:

        int connect(socket_addr& address);
        int connect(socket_addr&& address);

        int listen(int backLog = 0x7fffffff);
        tcp_socket accept(socket_addr& fromAddress);

        int send(const byte* data, int dataLength);
        int receive(byte* buffer, int bufferLength);

    private:

        tcp_socket(uint socket);

        friend tcp_socket CreateTCPSocket(address_family);
    };
}

