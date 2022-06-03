#pragma once

#include <fstream>
#include <array>
#include <vector>
#include "../../DesktopEditor/common/Types.h"


namespace CFCPP
{
class StreamRW
{
public:
    StreamRW(std::fstream &stream);
    T_LONG64 Seek(T_LONG64 offset);

    template <class T>
    T Read();

    template<class T>
    void Write(T value);

    std::vector<BYTE> ReadArray(int lenght);
    void WriteArray(BYTE *arr, int lenght);

private:
    std::array<BYTE,8> buffer;
    std::fstream& stream;
};

}
