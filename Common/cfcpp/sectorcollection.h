#pragma once

#include "sector.h"
#include <list>
#include "svector.h"
#include "event.h"


namespace CFCPP
{
using Ver3SizeLimitReached = std::function<void()>;

class SectorCollection
{
public:
    std::vector<SVector<Sector>> largeArraySlices;
    SectorCollection();
    void Add(std::shared_ptr<Sector> item);
    void Clear();
    inline int Count()const {return count;}
    std::shared_ptr<Sector>& operator[](size_t index);
    Event<Ver3SizeLimitReached> OnVer3SizeLimitReached;

private:
    void DoCheckSizeLimitReached();
    int add(std::shared_ptr<Sector> item);

private:
    const int MAX_SECTOR_V4_COUNT_LOCK_RANGE = 524287; // 0x7FFFFF00 for Version 4
    const int SLICE_SIZE = 4096;
    bool sizeLimitReached = false;

    int count = 0;
};
}
