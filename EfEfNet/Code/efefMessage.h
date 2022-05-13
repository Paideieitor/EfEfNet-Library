#pragma once

#include "efefGlobals.h"

namespace efef
{
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
        socket_addr sender;

        message() : type(message_type::EMPTY), ID(0u), data(nullptr), size(0u), time(0l), sender(socket_addr()) {}
        void destroy()
        {
            type = EMPTY;
            ID = 0u;
            delete[] data;
            size = 0u;
            time = 0l;
            sender.fill(0u);
        }
    };
}
