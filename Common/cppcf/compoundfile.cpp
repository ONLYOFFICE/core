#include "compoundfile.h"

using namespace CFCPP;

CompoundFile::CompoundFile() :
    CompoundFile(CFSVersion::Ver_3, CFSConfiguration::Default)
{}

CompoundFile::CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags)
{

}

void CompoundFile::OnSizeLimitReached()
{
    Sector rangeLockSector = new Sector(GetSectorSize(), sourceStream);
    sectors.Add(rangeLockSector);

    rangeLockSector.Type = SectorType.RangeLockSector;

    _transactionLockAdded = true;
    _lockSectorId = rangeLockSector.Id;
}

int CompoundFile::GetSectorSize()
{

    return 2 << (header.sectorShift - 1);
}
