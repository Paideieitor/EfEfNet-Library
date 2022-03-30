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

        int send();
        int send_to();
        int receive();
        int receive_from();

        acces_state accesState = PUBLIC;

    private:

        fast_socket(uint socket);

        socket_addr mAddress;
        efef::set<socket_addr> list;

        friend fast_socket CreateFastSocket(address_family);
	};
}