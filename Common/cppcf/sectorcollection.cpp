#include "sectorcollection.h"

using namespace CFCPP;

SectorCollection::SectorCollection()
{

}

void SectorCollection::Add(const Sector &item)
{
    if (DoCheckSizeLimitReached() == false)
        return;

    add(item);
}

void SectorCollection::Clear()
{
    largeArraySlices.clear();
    count = 0;
}

bool SectorCollection::DoCheckSizeLimitReached()
{
    if (!sizeLimitReached && (count - 1 > MAX_SECTOR_V4_COUNT_LOCK_RANGE))
    {
        sizeLimitReached = true;
        return false;
    }
    return true;
}

int SectorCollection::add(const Sector &item)
{
    unsigned itemIndex = count / SLICE_SIZE;

    if (itemIndex < largeArraySlices.size())
    {
        largeArraySlices[itemIndex]->push_back(item);
        count++;
    }
    else
    {
        std::unique_ptr<std::vector<Sector>> ar(new std::vector<Sector>(SLICE_SIZE));
        ar->emplace_back(item);
        largeArraySlices.push_back(ar);
        count++;
    }

    return count - 1;
}
