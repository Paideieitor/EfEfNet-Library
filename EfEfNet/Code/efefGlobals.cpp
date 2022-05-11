#include "efefGlobals.h"

void efef::memory_set(void* const dest, byte value, uint amount)
{
	for (uint i = 0; i < amount; ++i)
		reinterpret_cast<byte* const>(dest)[i] = value;
}

void efef::memory_copy(const void* const src, void* const dest, uint amount)
{
	for (uint i = 0; i < amount; ++i)
		reinterpret_cast<byte* const>(dest)[i] = reinterpret_cast<const byte* const>(src)[i];
}
