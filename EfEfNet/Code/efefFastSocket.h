#pragma once

#include "efefSocket.h"
#include "efefSocketAddr.h"
#include "efefSocketSet.h"
#include "efefInputStream.h"

#define RECVID_SIZE 16

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

        void send(const byte* data, int dataLength);
        int receive(byte* buffer, int bufferLength);

        void add_to_list(const socket_addr& fromAddress);
        void erase_from_list(const socket_addr& fromAddress);
        void erase_from_list(uint index);

        void erase_list();

        void update();

        acces_state accesState = PUBLIC;

    private:

        enum message_type
        {
            NULL,
            MESSAGE,
            CONNECT,
            ACCEPT,
            DISCONNECT
        };

        struct message
        {
            message_type type;
            uint ID;
            byte* data;

            message() : type(message_type::NULL), ID(0u), data(nullptr) {}
            message(message_type type, uint ID, byte* data) : type(type), ID(ID), data(data) {}
        };

        fast_socket(uint socket);

        void send_message();

        uint next_id;
        uint recv_ids[RECVID_SIZE];

        istream to_send;

        socket_addr mAddress;
        efef::set<socket_addr> recvList;

        friend fast_socket CreateFastSocket(address_family);
	};
}