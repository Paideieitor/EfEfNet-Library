#pragma once

#include "efefTypes.h"

namespace efef
{
	class ostream
	{
	public:

		ostream(byte* data, uint size, bool dispose = false);
		ostream(const byte* data, uint size, bool dispose = false);
		~ostream();

		template <class T>
		T GetVar()
		{
			byte data[sizeof(T)];

			uint start = current;
			for (uint i = 0u; i < sizeof(T) && current < size; ++i)
			{
				data[i] = stream[start + i];
				++current;
			}

			return *reinterpret_cast<T*>(data);
		}

		template <class T>
		T* GetArray(T* array, uint size)
		{
			for (uint i = 0; i < size; ++i)
				array[i] = GetVar<T>();

			return array;
		}

		// This function allocates memory, make sure to release it!
		template <class T>
		T* DynamicGetArray(uint size)
		{
			T* array = new T[size];

			for (uint i = 0; i < size; ++i)
				array[i] = GetVar<T>();

			return array;
		}

		uint Size() const;

		bool dispose;

	private:

		uint current;
		uint size;
		const byte* const stream;
	};
}