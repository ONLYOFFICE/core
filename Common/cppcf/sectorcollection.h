#pragma once

#include "sector.h"
#include <list>

namespace CFCPP
{
class SectorCollection
{
public:
    std::list<std::vector<Sector>> largeArraySlices;
    SectorCollection();
    void Add(Sector item);

private:
    bool DoCheckSizeLimitReached();
    int add(Sector item);
private:
    const int MAX_SECTOR_V4_COUNT_LOCK_RANGE = 524287; //0x7FFFFF00 for Version 4
    const int SLICE_SIZE = 4096;
    bool sizeLimitReached = false;

    int count = 0;

};
}
