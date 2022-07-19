#pragma once

#include <iostream>
#include <list>
#include "sector.h"
#include "slist.h"
#include "svector.h"

namespace CFCPP
{
class StreamView
{
public:
    StreamView(const SVector<Sector> &sectorChain, int sectorSize, Stream stream);
    StreamView(const SVector<Sector> &sectorChain, int sectorSize, std::streamsize length,
               SList<Sector> &availableSectors, Stream stream, bool isFatStream = false);

    void Write(const char *buffer, std::streamsize offset, std::streamsize count);
    std::streamsize Read(char *buffer, std::streamsize offset, std::streamsize count);
    std::streamsize Seek(std::streamsize offset, int origin);
    void SetLength(std::streamsize value);
    std::streamsize getLength() const;
    inline SVector<Sector>& BaseSectorChain() {return sectorChain;}

    int ReadInt32();
    void WriteInt32(int val);

private:
    void adjustLength(std::streamsize value);
    void adjustLength(std::streamsize value, SList<Sector> &availableSectors);
private:
    int sectorSize;
    std::streamsize length;

    SVector<Sector> sectorChain;
    Stream stream;
    bool isFatStream = false;
    int buf = 0;

public:
    SList<Sector> freeSectors;
    std::streamsize position;
};
}
