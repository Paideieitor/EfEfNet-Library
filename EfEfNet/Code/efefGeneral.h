#pragma once

#include "efefUDPSocket.h"
#include "efefTCPSocket.h"
#include "efefFastSocket.h"

namespace efef
{
	typedef set<udp_socket*> udp_set;
	typedef set<tcp_socket*> tcp_set;
	typedef set<fast_socket*> fast_set;

	int Init();
	int CleanUp();

	int GetLastError();

	udp_socket CreateUDPSocket(address_family family);
	tcp_socket CreateTCPSocket(address_family family);
	fast_socket CreateFastSocket(address_family family);
	fast_socket* CreateDynmcFastSocket(address_family family);

	int Poll(udp_socket* const socket, select_mode mode, long millisec = -1);
	int Select(udp_set* const sockets, select_mode mode, long millisec = -1);
	int Select(udp_set* const receive, udp_set* const send, udp_set* const except, long millisec = -1);

	int Poll(tcp_socket* const socket, select_mode mode, long millisec = -1);
	int Select(tcp_set* const sockets, select_mode mode, long millisec = -1);
	int Select(tcp_set* const receive, tcp_set* const send, tcp_set* const except, long millisec = -1);
}