#pragma once

#include <fstream>
#include <array>
#include <vector>
#include "Stream.h"


namespace CFCPP
{
class StreamRW
{
public:
    StreamRW(const Stream &stream);
    T_LONG64 Seek(T_LONG64 offset);

    template <class T>
    T Read();

    template<class T>
    void Write(T value);

    std::vector<BYTE> ReadArray(int lenght);
    void ReadArray(char* data, int lenght);
    void ReadArray(BYTE* data, int lenght);
    template<size_t N>
    std::array<BYTE, N> ReadArray();
    void WriteArray(const BYTE *arr, int lenght);
    void WriteArray(const char *arr, int lenght);

    inline void Close(){return;}

private:
    std::array<BYTE,8> buffer;
    Stream stream;
};

}
