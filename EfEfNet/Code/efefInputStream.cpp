#include "efefInputStream.h"

#include <vcruntime_string.h>

efef::istream::istream() : size(0u), capacity(1u), stream(new byte[capacity]) {}

efef::istream::~istream() { delete[] stream; }

const uint efef::istream::Size() const { return size; }

const byte* const efef::istream::Buffer() const { return stream; }

void efef::istream::Push(const byte& b)
{
	if (size >= capacity)
		Expand();

	stream[size++] = b;
}

void efef::istream::Expand()
{
	byte* newStream = new byte[capacity * 2];

	//for (uint i = 0; i < capacity; ++i)
	//	newStream[i] = stream[i];
	memcpy(newStream, stream, capacity);

	delete[] stream;
	stream = newStream;
	capacity *= 2;
}
