#include "efefOutputStream.h"

#include "efefPrivateGlobals.h"

efef::ostream::ostream(byte* data, uint size, bool dispose) : mCurrent(0u), mSize(size), mStream(data), dispose(dispose) {}
efef::ostream::ostream(const byte* data, uint size, bool dispose) : mCurrent(0u), mSize(size), mStream(data), dispose(dispose) {}

efef::ostream::~ostream() { if (dispose) delete[] mStream; }

uint efef::ostream::size() const { return mSize; }

#ifdef EFEF_DEBUG

#include <iostream>
#include <string>
void efef::ostream::Print()
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

void efef::ostream::Print() {}

#endif // EFEF_DEBUG
