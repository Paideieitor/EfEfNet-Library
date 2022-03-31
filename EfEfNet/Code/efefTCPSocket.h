#pragma once

#include "efefSocket.h"

namespace efef
{
    class tcp_socket : public efef_socket
    {
    public:

        int connect(socket_addr& address) const;
        int connect(socket_addr&& address) const;

        int listen(int backLog = 0x7fffffff) const;
        tcp_socket accept(socket_addr& fromAddress) const;

        int send(const byte* data, int dataLength) const;
        int receive(byte* buffer, int bufferLength) const;

    private:

        tcp_socket(uint socket);

        friend tcp_socket CreateTCPSocket(address_family);
    };
}
