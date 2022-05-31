#pragma once

namespace CFCPP
{
enum CFSConfiguration
{

    Default = 1,
    SectorRecycle = 2,
    EraseFreeSectors = 4,
    NoValidationException = 8,
    LeaveOpen = 16,
};

class CompoundFile
{
public:
    CompoundFile();

public:
    CFSConfiguration configuration = Default;
};
}
