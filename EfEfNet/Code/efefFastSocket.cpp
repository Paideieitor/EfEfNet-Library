#include "efefFastSocket.h"

#include "efefPrivateGlobals.h"
#include "efefInputStream.h"
#include "efefManager.h"

efef::fast_socket::fast_socket(uint socket) : efef_socket(socket), nextID(10u) 
{
    lastSentTime = GetTickCount();
    mAddress.fill(0u);
    mRemote.fill(0u);

    efef::manager::instance()->sockets.add(this);
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

void efef::fast_socket::send(const byte* data, uint dataLength)
{
    if (mRemote == false)
        return;

    if (toSend.size() == 0u)
        toSend.push_var("\\");

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

efef::set<efef::fast_socket::message> efef::fast_socket::receive()
{
    efef::set<efef::fast_socket::message> output(messages.capacity());
    output.swap(messages);

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
            send_message();
            lastSentTime = GetTickCount();
        }

        // RESEND
        currentTime = GetTickCount();
        for (uint i = 0; i < unaknowledged.size(); ++i)
            if (currentTime - unaknowledged[i].time >= resend_wait_time)
            {
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

void efef::fast_socket::send_ID(uint ID)
{
    if (toSend.size() == 0u)
        toSend.push_var("\\");
    toSend.push_var(ID);
}

void efef::fast_socket::resend(efef::fast_socket::message& msg)
{
    if (toSend.size() == 0u)
        toSend.push_var("\\");

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
        messages.add(msg);
        efef::DebugError("Fast Socket Recieve Messages Error");
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
    for (uint i = 0; i < (uint)bufferLength; ++i)
        if (buffer[i] == '\\')
        {
            uint ID = *reinterpret_cast<uint*>(buffer[head]);
            head += sizeof(uint);
            
            switch (ID)
            {
            case 0: // DISCONNECT
                send_ID(1u);
                disconnect_socket();
                break;
            case 1: // DISCONNECT AKNOWLEDGE
                disconnect_socket();
                break;
            default:
                uint size = i - head;
                if (size > 0) // NORMAL MESSAGE
                {
                    message msg;
                    msg.ID = ID;
                    msg.type = MESSAGE;
                    msg.size = size;
                    msg.data = new byte[msg.size];
                    memory_copy(buffer + head, msg.data, msg.size);

                    messages.add(msg);
                    send_ID(msg.ID);
                }
                else // AKNOWLEDGEMENT
                    for (uint j = 0; j < unaknowledged.size(); ++j)
                        if (ID == unaknowledged[j].ID)
                        {
                            delete[] unaknowledged[j].data;
                            unaknowledged.erase(j);
                            break;
                        }
                break;
            }

            head = i + 1;
        }
}

void efef::fast_socket::disconnect_socket()
{
    mAddress.fill(0u);

    message msg;
    msg.type = DISCONNECT;
    messages.add(msg);
}
