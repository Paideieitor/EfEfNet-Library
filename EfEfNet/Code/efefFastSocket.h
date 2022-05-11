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

        enum access_state
        {
            PUBLIC,
            ACCESS_LIST,
            BAN_LIST
        };

        enum message_type
        {
            EMPTY,
            MESSAGE,
            DISCONNECT,
            FAIL
        };

        struct message
        {
            message_type type;
            uint ID;
            byte* data;
            uint size;
            ulong time;

            message() : type(message_type::EMPTY), ID(0u), data(nullptr), size(0u), time(0l) {}
            void destroy() 
            {
                type = EMPTY;
                ID = 0u;
                delete[] data;
                size = 0u;
                time = 0l;
            }
        };

        int bind(socket_addr&) override;
        int bind(socket_addr&&) override;

        void send(const byte* data, uint dataLength);
        // WARNING: messages on the set must be destroyed to free their memory, use the destroy() member fuction 
        efef::set<message> receive();

        void disconnect();

        void add_to_list(const socket_addr& fromAddress);
        void erase_from_list(const socket_addr& fromAddress);
        void erase_from_list(uint index);

        void erase_list();

        access_state accessState = PUBLIC;
        uint listen_backlog = 5u;
        ulong send_rate_time = 10l;
        ulong resend_wait_time = 200l;

        socket_addr mRemote;

    private:

        fast_socket(uint socket);

        void update();

        void send_ID(uint ID);
        void resend(message& msg);
        void send_message();
        void receive_message();
        void disconnect_socket();

        // Send
        uint nextID; // IDs start at 10 (0-9 reserved numbers)
        istream toSend;
        ulong lastSentTime;

        // Receive
        efef::set<message> unaknowledged;
        efef::set<message> messages;

        // General
        socket_addr mAddress;
        efef::set<socket_addr> recvList;

        friend fast_socket CreateFastSocket(address_family);
        friend class manager;
	};
}