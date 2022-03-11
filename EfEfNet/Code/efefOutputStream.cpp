#include "efefOutputStream.h"

efef::ostream::ostream(byte* data, uint size, bool dispose) : mCurrent(0u), mSize(size), mStream(data), dispose(dispose) {}
efef::ostream::ostream(const byte* data, uint size, bool dispose) : mCurrent(0u), mSize(size), mStream(data), dispose(dispose) {}

efef::ostream::~ostream() { if (dispose) delete[] mStream; }

uint efef::ostream::size() const { return mSize; }
