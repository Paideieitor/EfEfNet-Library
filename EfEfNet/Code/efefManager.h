#pragma once

#include "efefGeneral.h"
#include "efefServer.h"

namespace efef
{
	class manager
	{
	public:

		static manager* const instance();

		void update();

		fast_set sockets;
		efef::set<server*> servers;

	private:

		manager();
		~manager();

		static manager* inst;

		friend int Init();
		friend int CleanUp();
	};
}
