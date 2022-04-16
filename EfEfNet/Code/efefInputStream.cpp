#include "efefInputStream.h"

#include "efefPrivateGlobals.h"

efef::istream::istream() : mSize(0u), mCapacity(2u), mStream(new byte[mCapacity]) {}

efef::istream::istream(const istream& stream) : mSize(stream.mSize), mCapacity(stream.mCapacity), mStream(new byte[stream.mCapacity])
{
	memory_copy(stream.mStream, mStream, mCapacity);
}

efef::istream::~istream() { delete[] mStream; }

const uint efef::istream::size() const { return mSize; }

const byte* const efef::istream::get_buffer() const { return mStream; }

void efef::istream::clear(bool flush)
{
	if (flush)
	{
		delete[] mStream;

		mCapacity = 2u;
		mStream = new byte[mCapacity];
	}
		
	mSize = 0u;
}

void efef::istream::push(const byte& b)
{
	while (mSize >= mCapacity)
		expand_array(mStream, mCapacity);

	mStream[mSize++] = b;
}