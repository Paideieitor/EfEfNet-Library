#pragma once

#include "efefTypes.h"

namespace efef
{
	class istream
	{
	public:

		istream();
		~istream();

		template <class T>
		void PushVar(T t)
		{
			byte* bytes = reinterpret_cast<byte*>(&t);
			for (uint i = 0u; i < sizeof(T); ++i)
				Push(bytes[i]);
		}

		template <class T>
		void PushArray(T t, uint size)
		{
			for (uint i = 0u; i < size; ++i)
				PushVar(t[i]);
		}

		const uint Size() const;

		const byte* const Buffer() const;

	private:

		void Push(const byte& b);

		void Expand();

		uint size;
		uint capacity;
		byte* stream;
	};
}
