#pragma once

#define EFEF_DEBUG

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include "efefDebug.h" // <- includes efefGlobals.h

namespace efef
{
	typedef char address[ADDR_SIZE];

	sockaddr* GetAddress(address& addr);
	sockaddr_in* GetIPv4(address& addr);

	class priv_util
	{
	public:

		template <class T>
		static fd_set CreateWS2Set(T* const tSockets)
		{
			fd_set set;
			FD_ZERO(&set);

			if (tSockets)
				for (uint i = 0; i < tSockets->size(); ++i)
					FD_SET((*tSockets)[i]->mSocket, &set);

			return set;
		}

		template <class T>
		static void FillefefSet(T* const tSockets, const fd_set& set)
		{
			if (!tSockets)
				return;

			for (uint i = 0; i < tSockets->size(); ++i)
				if (!FD_ISSET((*tSockets)[i]->mSocket, &set))
					tSockets->erase(i);
		}

		static bool ReceivePoll(SOCKET socket)
		{
			int output = 0;

			fd_set set;
			FD_ZERO(&set);
			FD_SET(socket, &set);

			timeval* waitPtr = nullptr;
			return (bool)select(0, &set, nullptr, nullptr, waitPtr);
		}

		template <class T>
		static int Select(T* const sockets, select_mode mode, long millisec)
		{
			fd_set set = priv_util::CreateWS2Set<T>(sockets);

			timeval* waitPtr = nullptr;
			timeval waitTime;
			if (millisec >= 0)
			{
				waitTime.tv_sec = 0l;
				waitTime.tv_usec = millisec;
				waitPtr = &waitTime;
			}

			int output = 0;
			switch (mode)
			{
			case efef::RECEIVE:
				output = select(0, &set, nullptr, nullptr, waitPtr);
				break;
			case efef::SEND:
				output = select(0, nullptr, &set, nullptr, waitPtr);
				break;
			case efef::EXCEPT:
				output = select(0, nullptr, nullptr, &set, waitPtr);
				break;
			}

			if (output > 0)
				priv_util::FillefefSet(sockets, set);
			else
				sockets->clear();

			return output;
		}

		template <class T>
		static int Select(T* const receive, T* const send, T* const except, long millisec)
		{
			fd_set recvSet = priv_util::CreateWS2Set<T>(receive);
			fd_set sendSet = priv_util::CreateWS2Set<T>(send);
			fd_set excpSet = priv_util::CreateWS2Set<T>(except);

			timeval* waitPtr = nullptr;
			timeval waitTime;
			if (millisec >= 0)
			{
				waitTime.tv_sec = 0l;
				waitTime.tv_usec = millisec;
				waitPtr = &waitTime;
			}

			int output = select(0, &recvSet, &sendSet, &excpSet, waitPtr);

			if (output > 0)
			{
				priv_util::FillefefSet(receive, recvSet);
				priv_util::FillefefSet(send, sendSet);
				priv_util::FillefefSet(except, excpSet);
			}

			return output;
		}
	};
	
}

// Winsock socket function wrap
// this allows to name the fuctions on my classes the same as the winsock functions
int ws2bind(SOCKET s, const sockaddr* name, int namelen);
int ws2connect(SOCKET s, const sockaddr* name, int namelen);
int ws2listen(SOCKET s, int backlog);
SOCKET ws2accept(SOCKET s, sockaddr* addr, int* addrlen);
int ws2send(SOCKET s, const char* buf, int len, int flags);
