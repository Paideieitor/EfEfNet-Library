#pragma once

namespace efef
{
	template <class T>
	class set
	{
	public:

		set(uint capacity = 0u) : mSize(0u), mCapacity(capacity), mData(new T[mCapacity]) {}
		~set() { delete mData; }

		void add(const T& t)
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

		void clear(bool flush = false) 
		{ 
			if (flush)
			{
				delete[] mData;

				mCapacity = 2u;
				mData = new T[mCapacity];
			}

			mSize = 0u; 
		}

		void swap(set<T>& set)
		{
			uint bSize = mSize;
			uint bCapacity = mCapacity;
			T* bData = mData;

			mSize = set.mSize;
			mCapacity = set.mCapacity;
			mData = set.mData;

			set.mSize = bSize;
			set.mCapacity = bCapacity;
			set.mData = bData;
		}

		T at(uint index)
		{
			return index < mSize ? mData[index] : mData[-1];
		}

		T* get_ptr(uint index)
		{
			return index < mSize ? mData + index : nullptr;
		}

		uint size() const { return mSize; }
		uint capacity() const { return mCapacity; }

		T& operator[](uint index) const { return index < mSize ? mData[index] : mData[-1]; }

	private:

		uint mSize;
		uint mCapacity;
		T* mData;
	};
}
