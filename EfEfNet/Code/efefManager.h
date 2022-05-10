#pragma once

#include "efefGeneral.h"

namespace efef
{
	class manager
	{
	public:

		static manager* const instance();

		void update();

		fast_set sockets;

	private:

		manager();
		~manager();

		static manager* inst;

		friend int Init();
		friend int CleanUp();
	};
}
