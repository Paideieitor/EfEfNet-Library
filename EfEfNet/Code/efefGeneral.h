#pragma once

#include "efefSocketSet.h"

namespace efef
{
	int Init();
	int CleanUp();

	int GetLastError();

	udp_socket CreateUDPSocket(address_family family);
	tcp_socket CreateTCPSocket(address_family family);

	int Poll(const udp_socket* socket, select_mode mode, long millisec = -1);
	int Select(udp_set* const sockets, select_mode mode, long millisec = -1);
	int Select(udp_set* const receive, udp_set* const send, udp_set* const except, long millisec = -1);

	int Poll(const tcp_socket* socket, select_mode mode, long millisec = -1);
	int Select(tcp_set* const sockets, select_mode mode, long millisec = -1);
	int Select(tcp_set* const receive, tcp_set* const send, tcp_set* const except, long millisec = -1);
}