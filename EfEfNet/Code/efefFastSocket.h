#pragma once

#include "efefSocket.h"
#include "efefSocketAddr.h"
#include "efefSocketSet.h"

namespace efef
{

	class fast_socket : public efef_socket
	{
    public:

        enum acces_state
        {
            PUBLIC,
            ACCES_LIST,
            BAN_LIST
        };

        int bind(socket_addr&);
        int bind(socket_addr&&);

        int connect(socket_addr& address);
        fast_socket accept(socket_addr& fromAddress);

        int send(const byte* data, int dataLength);
        int receive(byte* buffer, int bufferLength);

        void add_to_list(const socket_addr& fromAddress);
        void erase_from_list(const socket_addr& fromAddress);
        void erase_from_list(uint index);

        void erase_list();

        acces_state accesState = PUBLIC;

    private:

        uint next_id = 0u;
        uint recv_ids[16];

        fast_socket(uint socket);

        socket_addr mAddress;
        efef::set<socket_addr> recvList;

        friend fast_socket CreateFastSocket(address_family);
	};
}