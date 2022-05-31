#pragma once
#include "compoundfile.h"

namespace CFCPP
{
class CFItem
{
public:
    CFItem();

protected:
    void CheckDisposed()
    {
        if (compoudFile.SizeFile() == 0)
    }

public:
    CompoundFile compoudFile;
};
}
