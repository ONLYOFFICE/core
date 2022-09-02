#pragma once


#include "sector.h"

namespace CFCPP
{
class Header
{
public:
    Header();
    Header(ushort version);
    void Write(Stream& stream);
    void Read(Stream& stream);

private:
    void CheckVersion()const;
    void CheckSignature()const;

public:
    BYTE headerSignature[8] = {0xD0, 0xCF, 0x11, 0xE0, 0xA1, 0xB1, 0x1A, 0xE1};
    BYTE clsid[16];
    USHORT minorVersion = 0x003E;
    USHORT majorVersion = 0x0003;
    USHORT byteOrder = 0xFFFE;
    USHORT sectorShift = 9;
    USHORT miniSectorShift = 6;
    BYTE unUsed[6];
    INT directorySectorsNumber;
    INT fatSectorsNumber;
    INT firstDirectorySectorID = Sector::ENDOFCHAIN;
    uint unUsed2;
    uint minSizeStandardStream = 4096;
    INT firstMiniFATSectorID = 0xFFFFFFFE;
    uint miniFATSectorsNumber;
    INT firstDIFATSectorID = Sector::ENDOFCHAIN;
    uint difatSectorsNumber;
    INT difat[109];
};

}
