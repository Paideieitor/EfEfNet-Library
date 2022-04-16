#include "efefGlobals.h"

void efef::memory_set(void* src, byte value, uint amount)
{
	for (uint i = 0; i < amount; ++i)
		reinterpret_cast<byte*>(src)[i] = value;
}

void efef::memory_copy(void* src, void* dest, uint amount)
{
	for (uint i = 0; i < amount; ++i)
		reinterpret_cast<byte*>(dest)[i] = reinterpret_cast<byte*>(src)[i];
}
