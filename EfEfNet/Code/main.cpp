#include "efefNet.h"
#include "efefStreams.h"

#include <iostream>

int main()
{
	if (efef::Init() == EFEF_ERROR)
	{
		std::cout << "Init Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	if (efef::CleanUp() == EFEF_ERROR)
	{
		std::cout << "Clean Up Error: " << efef::GetLastError() << std::endl;
		return 0;
	}

	return 0;
}