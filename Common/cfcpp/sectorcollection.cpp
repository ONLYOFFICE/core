#include "sectorcollection.h"
#include "cfexception.h"

using namespace CFCPP;

SectorCollection::SectorCollection()
{

}

void SectorCollection::Add(std::shared_ptr<Sector> item)
{
    OnVer3SizeLimitReached();

    add(item);
}

void SectorCollection::Clear()
{
    largeArraySlices.clear();
    count = 0;
}

std::shared_ptr<Sector>& SectorCollection::operator[](size_t index)
{
    //    size_t globalPos = 0;
    //    for (size_t i = 0; i < largeArraySlices.size(); i ++)
    //    {
    //        size_t sliceSize = largeArraySlices[i].size();
    //        globalPos += sliceSize;
    //        if (globalPos < index)
    //            return largeArraySlices[i][i % sliceSize];
    //    }

    //    return {};
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
        //        std::unique_ptr<std::vector<Sector>> ar(new std::vector<Sector>(SLICE_SIZE));
        SVector<Sector> ar;
        ar.push_back(item);
        largeArraySlices.push_back(ar);
        count++;
    }

    return count - 1;
}
