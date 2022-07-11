#pragma once

#include "cfitem.h"

namespace CFCPP
{
class CFStream : public CFItem
{
public:
    CFStream(std::shared_ptr<CompoundFile> compFile, std::shared_ptr<IDirectoryEntry> dirEntry);
};
}
