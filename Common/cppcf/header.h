#pragma once


#include "sector.h"
#include <array>

namespace CFCPP
{
class Header
{
public:
    Header();

public:
    std::array<BYTE,8> headerSignature = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
    std::array<BYTE,16> clsid;
    USHORT minorVersion = 0x003E;
    USHORT majorVersion = 0x0003;
    USHORT byteOrder = 0xFFFE;
    USHORT sectorShift = 9;
    USHORT miniSectorShift = 6;
    std::array<BYTE,6> unused;
    int directorySectorsNumber;
    int fatSectorsNumber;
    int firstDirectorySectorID;
};

}
