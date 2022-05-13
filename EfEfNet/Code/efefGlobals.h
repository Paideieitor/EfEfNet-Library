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

	enum select_mode
	{
		RECEIVE,
		SEND,
		EXCEPT
	};

	template <class T>
	void expand_array(T*& src, uint& srcCapacity)
	{
		T* dest = new T[srcCapacity * sizeof(T) * 2u];

		for (uint i = 0; i < srcCapacity * sizeof(T); ++i)
			dest[i] = src[i];
		delete[] src;

		src = dest;
		srcCapacity *= 2;
	}

	void memory_set(void* const dest, byte value, uint amount);
	void memory_copy(const void* const src, void* const dest, uint amount);
	bool compare_str(char* str1, char* str2, uint size);
}