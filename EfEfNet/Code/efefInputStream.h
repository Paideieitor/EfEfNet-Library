#pragma once

#include "efefGlobals.h"

namespace efef
{
	class istream
	{
	public:

		istream();
		istream(const istream&);
		~istream();

		template <class T>
		void push_var(T t)
		{
			byte* bytes = reinterpret_cast<byte*>(&t);
			for (uint i = 0u; i < sizeof(T); ++i)
				push(bytes[i]);
		}

		template <class T>
		void push_array(T* t, uint tSize)
		{
			for (uint i = 0u; i < tSize; ++i)
				push_var(t[i]);
		}

		const uint size() const;

		const byte* const get_buffer() const;

		void clear(bool flush = false);

		void Print();

	private:

		void push(const byte& b);

		uint mSize;
		uint mCapacity;
		byte* mStream;
	};
}
