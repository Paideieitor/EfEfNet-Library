#pragma once

#include "efefUDPSocket.h"
#include "efefTCPSocket.h"

namespace efef
{
	template <class T>
	class set
	{
	public:

		set() : mSize(0u), mCapacity(2u), mData(new T[mCapacity]) {}
		~set() { delete mData; }

		void add(T t) 
		{
			while (mSize >= mCapacity)
				expand_array(mData, mCapacity);

			mData[mSize++] = t;
		}

		void erase(uint index)
		{
			for (uint i = index; i < mSize; ++i)
				mData[i] = mData[i + 1];
			--mSize;
		}

		uint size() const { return mSize; }
		uint capacity() const { return mCapacity; }

		T operator[](uint index) const { return index < mSize ? mData[index] : mData[-1]; }

	private:

		uint mSize;
		uint mCapacity;
		T* mData;
	};

	typedef set<udp_socket* const> udp_set;
	typedef set<tcp_socket* const> tcp_set;
}
