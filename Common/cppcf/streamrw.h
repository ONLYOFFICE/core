#pragma once

#include <fstream>
#include <array>
#include <vector>
#include "../../DesktopEditor/common/Types.h"
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
    template<size_t N>
    std::array<BYTE, N> ReadArray();
    void WriteArray(BYTE *arr, int lenght);

private:
    std::array<BYTE,8> buffer;
    Stream stream;
};

}
