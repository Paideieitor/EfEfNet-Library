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


#ifdef EFEF_DEBUG

#include <iostream>
#include <string>
void efef::istream::Print()
{
	char* toPrint = new char[mSize + 1u];
	memory_copy(mStream, toPrint, mSize);

	for (uint i = 0u; i < mSize; ++i)
		if (toPrint[i] == '\0')
			toPrint[i] = '0';
		else if (toPrint[i] == '\n')
			toPrint[i] = '1';

	toPrint[mSize] = '\0';

	std::string str(toPrint);
	std::cout << str << '\n';
	delete[] toPrint;
}

#else

void efef::istream::Print() {}

#endif // EFEF_DEBUG