#pragma once

#define EFEF_ERROR -1
#define EFEF_NO_ERROR 0

#define ADDR_SIZE 16u

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

typedef wchar_t wchar;

typedef const char* str;
typedef const wchar_t *wstr;

typedef unsigned char byte;

namespace efef
{
	enum address_family
	{
		IPv4,
		IPv6
	};
}
