#include "cfstream.h"
#include "cfexception.h"


CFCPP::CFStream::CFStream(std::shared_ptr<CompoundFile> compFile, std::shared_ptr<IDirectoryEntry> dirEntry) :
    CFItem(compFile)
{
    if (dirEntry == nullptr || dirEntry->getSid() < 0)
        throw new CFException("Attempting to create a CFStorage using an unitialized directory");

    this->dirEntry = dirEntry;
}
