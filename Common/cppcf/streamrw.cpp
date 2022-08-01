#include "streamrw.h"
#include <algorithm>

using namespace CFCPP;

StreamRW::StreamRW(const Stream &stream)
    : stream(stream)
{
    buffer.fill(0);
}

T_LONG64 StreamRW::Seek(T_LONG64 offset)
{
    stream->seekp(offset, std::ios::beg);
    return stream->tellp();
}

void StreamRW::ReadArray(char *data, int lenght)
{
    stream->read(data, lenght);
}

void StreamRW::ReadArray(BYTE* data, int lenght)
{
    stream->read(reinterpret_cast<char*>(data), lenght);
}

void StreamRW::WriteArray(const BYTE *arr, int lenght)
{
    stream->write(reinterpret_cast<const char*>(arr), lenght);
}

void StreamRW::WriteArray(const char *arr, int lenght)
{
    stream->write(arr, lenght);
}
