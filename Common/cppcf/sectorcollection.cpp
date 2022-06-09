#include "sectorcollection.h"

using namespace CFCPP;

SectorCollection::SectorCollection()
{

}

void SectorCollection::Add(Sector item)
{
    if (DoCheckSizeLimitReached() == false)
        return;

    add(item);
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

int SectorCollection::add(Sector item)
{
    int itemIndex = count / SLICE_SIZE;

                if (itemIndex < largeArraySlices.Count)
                {
                    largeArraySlices[itemIndex].Add(item);
                    count++;
                }
                else
                {
                    ArrayList ar = new ArrayList(SLICE_SIZE);
                    ar.Add(item);
                    largeArraySlices.Add(ar);
                    count++;
                }

                return count - 1;
}
