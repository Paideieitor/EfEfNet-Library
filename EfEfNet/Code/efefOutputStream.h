#pragma once

#include "efefGlobals.h"

namespace efef
{
	class ostream
	{
	public:

		ostream(byte* data, uint size, bool dispose = false);
		ostream(const byte* data, uint size, bool dispose = false);
		~ostream();

		template <class T>
		T get_var()
		{
			byte data[sizeof(T)];

			uint start = mCurrent;
			for (uint i = 0u; i < sizeof(T) && mCurrent < mSize; ++i)
			{
				data[i] = mStream[start + i];
				++mCurrent;
			}

			return *reinterpret_cast<T*>(data);
		}

		template <class T>
		T* get_array(T* t, uint tSize)
		{
			for (uint i = 0; i < tSize; ++i)
				t[i] = get_var<T>();

			return t;
		}

		// This function allocates memory, make sure to release it!
		template <class T>
		T* dynm_get_array(uint tSize)
		{
			T* t = new T[tSize];

			for (uint i = 0; i < tSize; ++i)
				t[i] = get_var<T>();

			return t;
		}

		uint size() const;

		bool dispose;

		void Print();

	private:

		uint mCurrent;
		uint mSize;
		const byte* const mStream;
	};
}