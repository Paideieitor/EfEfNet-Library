#pragma once

#include "efefSocket.h"
#include "efefSocketAddr.h"
#include "efefSocketSet.h"
#include "efefInputStream.h"
#include "efefMessage.h"

#define RECVID_SIZE 16u
#define ID_START 10u

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

        ~fast_socket();

        int bind(socket_addr&) override;
        int bind(socket_addr&&) override;

        void connect(const socket_addr& remoteAddress);

        bool poll(select_mode mode);

        void send(const byte* data, uint dataLength);
        void force_send();
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

        bool operator== (const fast_socket& socket) const;
        bool operator== (const bool& active) const;

    private:

        fast_socket(uint socket);

        void update();
        void clean_up();

        void send_ID(uint ID);
        void resend(message& msg);
        void send_message();
        void receive_message();
        void disconnect_socket(socket_addr& sender);

        // Send
        uint nextID; // IDs start at 10 (0-9 reserved numbers)
        istream toSend;
        ulong lastSentTime;

        // Receive
        efef::set<message> messages;
        efef::set<message> unaknowledged;
        efef::set<uint> recvIDs;

        // General
        socket_addr mAddress;
        socket_addr mRemote;
        efef::set<socket_addr> recvList;

        friend fast_socket CreateFastSocket(address_family);
        friend class manager;
	};
}