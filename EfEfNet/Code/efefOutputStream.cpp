#include "efefOutputStream.h"

efef::ostream::ostream(byte* data, uint size, bool dispose) : current(0u), size(size), stream(data), dispose(dispose) {}
efef::ostream::ostream(const byte* data, uint size, bool dispose) : current(0u), size(size), stream(data), dispose(dispose) {}

efef::ostream::~ostream() { if (dispose) delete[] stream; }

uint efef::ostream::Size() const { return size; }
