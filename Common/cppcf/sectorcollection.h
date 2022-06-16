#pragma once

#include "sector.h"
#include <list>
#include "svector.h"

namespace CFCPP
{

class SectorCollection
{
public:
    std::vector<SVector<Sector>> largeArraySlices;
    SectorCollection();
    void Add(const Sector &item);
    void Clear();
    inline int Count()const {return count;}

private:
    bool DoCheckSizeLimitReached();
    int add(const Sector &item);
private:
    const int MAX_SECTOR_V4_COUNT_LOCK_RANGE = 524287; //0x7FFFFF00 for Version 4
    const int SLICE_SIZE = 4096;
    bool sizeLimitReached = false;

    int count = 0;

};
}
