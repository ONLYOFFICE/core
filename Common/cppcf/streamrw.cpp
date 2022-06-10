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
    stream->seekg(offset);
    return stream->tellg();
}


template<size_t N>
std::array<BYTE, N> StreamRW::ReadArray()
{
    std::array<BYTE,N> arr(0);
    stream->read(reinterpret_cast<char*>(arr.data()), N);
    return arr;
}
std::vector<BYTE> StreamRW::ReadArray(int lenght)
{
    std::vector<BYTE> arr(lenght,0);
    stream->read(reinterpret_cast<char*>(arr.data()), lenght);
    return arr;
}

void StreamRW::WriteArray(BYTE *arr, int lenght)
{
    stream->write(reinterpret_cast<char*>(arr), lenght);
}

template<class T>
T StreamRW::Read()
{
    T value;
    char* asByteArr = reinterpret_cast<char*>(&value);
    stream->read(asByteArr, sizeof (T));
    std::reverse(asByteArr, asByteArr + sizeof (T));
    return value;
}

template<class T>
void StreamRW::Write(T value)
{
    char* asByteArr = reinterpret_cast<char*>(&value);
    std::reverse(asByteArr, asByteArr + sizeof (T));
    stream->write(asByteArr, sizeof (T));
}
