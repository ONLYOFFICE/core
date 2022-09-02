#pragma once

#include <fstream>
#include <array>
#include <vector>
#include "stream.h"


namespace CFCPP
{
class StreamRW
{
public:
    StreamRW(Stream stream);
    T_LONG64 Seek(T_LONG64 offset);
    T_LONG64 Tell();

    template <class T>
    T Read()
    {
        T value;
        char* asByteArr = reinterpret_cast<char*>(&value);
        stream->read(asByteArr, sizeof (T));
        return value;
    }

    template<class T>
    void Write(T value)
    {
        char* asByteArr = reinterpret_cast<char*>(&value);
        stream->write(asByteArr, sizeof (T));
    }

    void ReadArray(char* data, int lenght);
    void ReadArray(BYTE* data, int lenght);
    void WriteArray(const BYTE *arr, int lenght);
    void WriteArray(const char *arr, int lenght);

    inline void Close(){return;}

private:
    Stream stream;
};

}
