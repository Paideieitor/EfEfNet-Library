#pragma once

#include "efefSocketSet.h"

namespace efef
{
	int Init();
	int CleanUp();

	int GetLastError();

	udp_socket CreateUDPSocket(address_family family);
	tcp_socket CreateTCPSocket(address_family family);
}