#include "sectorcollection.h"
#include "cfexception.h"


using namespace CFCPP;

SectorCollection::SectorCollection()
{

}

void SectorCollection::Add(std::shared_ptr<Sector> item)
{
    DoCheckSizeLimitReached();

    add(item);
}

void SectorCollection::Clear()
{
    for (auto& slice : largeArraySlices)
        if (slice.isInit())
            slice->clear();

    count = 0;
}

std::shared_ptr<Sector>& SectorCollection::operator[](size_t index)
{
    int itemIndex = index / SLICE_SIZE;
    int itemOffset = index % SLICE_SIZE;

    return largeArraySlices[itemIndex][itemOffset];
}

void SectorCollection::DoCheckSizeLimitReached()
{
    if (OnVer3SizeLimitReached.size() && !sizeLimitReached && (count - 1 > MAX_SECTOR_V4_COUNT_LOCK_RANGE))
    {
        sizeLimitReached = true;
        OnVer3SizeLimitReached();
    }
}

int SectorCollection::add(std::shared_ptr<Sector> item)
{
    unsigned itemIndex = count / SLICE_SIZE;

    if (itemIndex < largeArraySlices.size())
    {
        largeArraySlices[itemIndex].push_back(item);
        count++;
    }
    else
    {
        SVector<Sector> ar(SLICE_SIZE);
        ar.push_back(item);
        largeArraySlices.push_back(ar);
        count++;
    }

    return count - 1;
}
