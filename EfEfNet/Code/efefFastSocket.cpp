#include "efefFastSocket.h"

#include "efefPrivateGlobals.h"
#include "efefInputStream.h"
#include "efefManager.h"

bool efef::fast_socket::operator==(const bool& active) const
{
    fast_socket* fs = const_cast<fast_socket*>(this);
    char* sck = reinterpret_cast<char*>(fs);
    for (uint i = 0; i < sizeof(fast_socket); ++i)
        if (sck[i] != 0)
            return active;
    return !active;
}

efef::fast_socket::fast_socket(uint socket) : efef_socket(socket), nextID(ID_START)
{
    lastSentTime = GetTickCount();
    mAddress.fill(0u);
    mRemote.fill(0u);
    toSend.size();

    efef::manager::instance()->sockets.add(this);
}

efef::fast_socket::~fast_socket()
{
    for (uint i = 0u; i < messages.size(); ++i)
        delete[] messages[i].data;
    for (uint i = 0u; i < unaknowledged.size(); ++i)
        delete[] unaknowledged[i].data;
    
    efef::manager* const manager = efef::manager::instance();
    uint index = 0u;
    for (uint i = 0u; i < manager->sockets.size(); ++i)
        if (manager->sockets[i] == this)
        {
            index = i;
            break;
        }
    efef::manager::instance()->sockets.erase(index);
}

int efef::fast_socket::bind(socket_addr& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Fast Socket Bind Error");

    mAddress = addr;
    return EFEF_NO_ERROR;
}

int efef::fast_socket::bind(socket_addr&& addr)
{
    int error = ws2bind((SOCKET)mSocket, efef::GetAddress(addr.mAddress), ADDR_SIZE);

    if (error < 0)
        return efef::DebugError("Fast Socket Bind Error");

    mAddress = addr;
    return EFEF_NO_ERROR;
}

void efef::fast_socket::connect(const socket_addr& remoteAddress)
{
    clean_up();
    mRemote = remoteAddress;
}

bool efef::fast_socket::poll(select_mode mode)
{
    switch (mode)
    {
    case efef::RECEIVE:
        if (mAddress == true && messages.size() > 0)
            return true;
        return false;
    case efef::SEND:
        if (mRemote == true && toSend.size() > 0)
            return true;
        return false;
    case efef::EXCEPT:
        break;
    }
    return false;
}

void efef::fast_socket::send(const byte* data, uint dataLength)
{
    if (mRemote == false)
        return;
    efef::DebugError("Sent Message");

    toSend.push_var(dataLength);

    uint ID = nextID++;
    toSend.push_var(ID);
    toSend.push_array(data, dataLength);

    message msg;
    msg.ID = ID;
    msg.type = MESSAGE;
    msg.size = dataLength;
    msg.data = new byte[dataLength];
    memory_copy(data, msg.data, dataLength);
    msg.time = GetTickCount();

    if (unaknowledged.size() > RECVID_SIZE)
    {
        delete[] unaknowledged[0u].data;
        unaknowledged.erase(0u);
    }
    unaknowledged.add(msg);
}

void efef::fast_socket::force_send()
{
    send_message();
    lastSentTime = GetTickCount();
}

efef::set<efef::message> efef::fast_socket::receive()
{
    efef::set<efef::message> output(messages.capacity());
    output.swap(messages);
    output.dontDelete = true;
    return output;
}

void efef::fast_socket::disconnect()
{
    send_ID(0u);
}

void efef::fast_socket::add_to_list(const socket_addr& address)
{
    for (uint i = 0; i < recvList.size(); ++i)
        if (address == recvList[i])
            return;

    recvList.add(address);
}

void efef::fast_socket::erase_from_list(const socket_addr& address)
{
    uint index = -1;
    for (uint i = 0; i < recvList.size(); ++i)
        if (address == recvList[i])
            index = i;
    if (index < 0)
        return;

    recvList.erase(index);
}

void efef::fast_socket::erase_from_list(uint index)
{
    if (index < 0 || index >= recvList.size())
        return;

    recvList.erase(index);
}

void efef::fast_socket::erase_list()
{
    recvList.clear();
}

void efef::fast_socket::update()
{
    if (mRemote == true)
    {
        // SEND
        ulong currentTime = GetTickCount();
        if (toSend.size() > 0 && currentTime - lastSentTime >= send_rate_time)
        {
            force_send();
        }

        // RESEND
        currentTime = GetTickCount();
        for (uint i = 0; i < unaknowledged.size(); ++i)
            if (currentTime - unaknowledged[i].time >= resend_wait_time)
            {
                DebugError("Resent Message");
                resend(unaknowledged[i]);
                unaknowledged[i].time = GetTickCount();
            }
    }

    if (mAddress == true)
    {
        // RECEIVE
        if (priv_util::ReceivePoll(mSocket))
            receive_message();
    }
}

void efef::fast_socket::clean_up()
{
    nextID = ID_START;
    toSend.clear(true);
    lastSentTime = GetTickCount();

    for (uint i = 0u; i < messages.size(); ++i)
        delete[] messages[i].data;
    messages.clear(true);

    for (uint i = 0u; i < unaknowledged.size(); ++i)
        delete[] unaknowledged[i].data;
    unaknowledged.clear(true);

    recvIDs.clear(true);
    recvList.clear(true);
}

void efef::fast_socket::send_ID(uint ID)
{
    toSend.push_var(0u);
    toSend.push_var(ID);
}

void efef::fast_socket::resend(efef::message& msg)
{
    toSend.push_var(msg.size);

    toSend.push_var(msg.ID);
    toSend.push_array(msg.data, msg.size);
}

void efef::fast_socket::send_message()
{
    int bytesSent = sendto(mSocket, (const char*)toSend.get_buffer(), toSend.size(), 0, efef::GetAddress(mRemote.mAddress), ADDR_SIZE);

    if (bytesSent < (int)toSend.size())
        efef::DebugError("Fast Socket Send Message To Error");

    toSend.clear();
}

void efef::fast_socket::receive_message()
{
    int bufferLength = 1024;
    char* buffer = new char[bufferLength];
    socket_addr sender;

    int senderLength = ADDR_SIZE;
    int bytesRead = recvfrom(mSocket, (char*)buffer, bufferLength, 0, efef::GetAddress(sender.mAddress), &senderLength);
    if (bytesRead < 0)
    {
        message msg;
        msg.type = FAIL;
        msg.sender = sender;
        messages.add(msg);
        efef::DebugError("Fast Socket Recieve Messages Error");
        return;
    }

    bool access = true;
    if (accessState != PUBLIC)
    {
        access = false;
        for (uint i = 0; i < recvList.size(); ++i)
            if (recvList[i] == sender)
                true;
        if (accessState == BAN_LIST)
            access = !access;
    }
    if (!access)
        efef::DebugError("Fast Socket Recieve Message Access Denied");

    uint head = 0;
    while (head < bytesRead)
    {
        uint size = *reinterpret_cast<uint*>(&buffer[head]);
        head += sizeof(uint);

        uint ID = *reinterpret_cast<uint*>(&buffer[head]);
        head += sizeof(uint);

        switch (ID)
        {
        case 0: // DISCONNECT
            send_ID(1u);
            force_send();
            disconnect_socket(sender);
            break;
        case 1: // DISCONNECT AKNOWLEDGE
            disconnect_socket(sender);
            break;
        default:
            if (size > 0u) // NORMAL MESSAGE
            {
                for (uint j = 0u; j < recvIDs.size(); ++j)
                    if (ID == recvIDs[j])
                    {
                        ID = 0u;
                        break;
                    }
                if (ID == 0u)
                    break;

                message msg;
                msg.ID = ID;
                msg.type = MESSAGE;
                msg.size = size;
                msg.data = new byte[msg.size];
                msg.sender = sender;
                memory_copy(buffer + head, msg.data, msg.size);

                if (recvIDs.size() > RECVID_SIZE)
                    recvIDs.erase(0u);
                recvIDs.add(msg.ID);

                messages.add(msg);
                send_ID(msg.ID);
                DebugError("Sent Aknowledgement");
            }
            else // AKNOWLEDGEMENT
                for (uint j = 0u; j < unaknowledged.size(); ++j)
                    if (ID == unaknowledged[j].ID)
                    {
                        DebugError("Received Aknowledgement");
                        delete[] unaknowledged[j].data;
                        unaknowledged.erase(j);
                        break;
                    }
            break;
        }
        head += size;
    }

    delete[] buffer;
}

void efef::fast_socket::disconnect_socket(socket_addr& sender)
{
    mRemote.fill(0u);
    clean_up();

    message msg;
    msg.type = DISCONNECT;
    msg.sender = sender;
    messages.add(msg);
}
