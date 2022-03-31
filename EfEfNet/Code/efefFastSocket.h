#pragma once

#include "efefSocket.h"
#include "efefSocketAddr.h"
#include "efefSocketSet.h"

namespace efef
{
    typedef efef::set<socket_addr> addr_set;

	class fast_socket : public efef_socket
	{
    public:

        enum acces_state
        {
            PUBLIC,
            PRIVATE
        };

        void connect(const socket_addr& addr);
        void accept(socket_addr& addr);

        void give_acces(const socket_addr& addr);
        void ban(const socket_addr& addr);

        int send(const byte* data, int dataLength);
        int receive(byte* buffer, int bufferLength, socket_addr& sender);

        acces_state accesState = PUBLIC;

    private:

        struct Message
        {
            enum messageType
            {
                CONNECT,
                ACCEPT,
                MESSAGE,
                ACKNOWLEDGE,
                DISCONNECT
            };
        };

        fast_socket(uint socket);

        int send_message(const byte* data, int dataLength, socket_addr& receiver) const;
        int receive_message(const byte* buffer, int bufferLength, socket_addr& sender) const;

        addr_set accesList;
        addr_set banList;

        uint currentID = 0u;

        friend fast_socket CreateFastSocket(address_family);
	};
}