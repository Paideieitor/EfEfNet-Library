#include "efefGlobals.h"

void efef::memory_set(void* const dest, byte value, uint amount)
{
	for (uint i = 0u; i < amount; ++i)
		reinterpret_cast<byte* const>(dest)[i] = value;
}

void efef::memory_copy(const void* const src, void* const dest, uint amount)
{
	for (uint i = 0u; i < amount; ++i)
		reinterpret_cast<byte* const>(dest)[i] = reinterpret_cast<const byte* const>(src)[i];
}

bool efef::compare_str(char* str1, char* str2, uint size)
{
	for (uint i = 0u; i < size; ++i)
		if (str1[i] != str2[i])
			return false;
	return true;
}
