#include "sectorcollection.h"

using namespace CFCPP;

SectorCollection::SectorCollection()
{

}

void SectorCollection::Add(std::shared_ptr<Sector> item)
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

std::shared_ptr<Sector> SectorCollection::operator[](size_t index)
{
    size_t globalPos = 0;
    for (size_t i = 0; i < largeArraySlices.size(); i ++)
    {
        size_t sliceSize = largeArraySlices[i].size();
        globalPos += sliceSize;
        if (globalPos < index)
            return largeArraySlices[i][i % sliceSize];
    }

    return {};
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
//        std::unique_ptr<std::vector<Sector>> ar(new std::vector<Sector>(SLICE_SIZE));
        SVector<Sector> ar;
        ar.push_back(item);
        largeArraySlices.push_back(ar);
        count++;
    }

    return count - 1;
}
