#include "cfitem.h"
#include "cfexception.h"

using namespace CFCPP;


int CFItem::CompareTo(const CFItem &other) const
{
    return dirEntry->CompareTo(other.dirEntry);
}

bool CFItem::operator==(const CFItem &rightItem) const
{
    return CompareTo(rightItem) == 0;
}

bool CFItem::operator!=(const CFItem &rightItem) const
{
    return CompareTo(rightItem) != 0;
}

int CFItem::GetHashCode() const
{
    return dirEntry->GetHashCode();
}

std::wstring CFItem::Name() const
{
    auto n = dirEntry->GetEntryName();
    if (n.empty() == false)
    {
        auto remIter = n.find_last_of('\0');
        if (remIter != std::wstring::npos)
            n.erase(remIter);
        return n;
    }
    else
        return L"";
}

bool CFItem::IsStorage() const
{
    return dirEntry->getStgType() == StgType::StgStorage;
}

bool CFItem::IsStream() const
{
    return dirEntry->getStgType() == StgType::StgStream;
}

bool CFItem::ISRoot() const
{
    return dirEntry->getStgType() == StgType::StgRoot;
}

GUID CFItem::getStorageCLSID() const
{
    return dirEntry->getStorageCLSID();
}

void CFItem::setStorageCLSID(GUID value)
{
    dirEntry->setStorageCLSID(value);
}

int CFItem::CompareTo(const CFItem &other)
{
    return dirEntry->CompareTo(other.dirEntry);
}

std::wstring CFItem::ToString() const
{
    if (dirEntry != nullptr)
        return L"[" + std::to_wstring(dirEntry->getLeftSibling()) + L"," + std::to_wstring(dirEntry->getSid()) + L"," +
                std::to_wstring(dirEntry->getRightSibling()) + L"] " + dirEntry->GetEntryName();
    else
        return L"";
}

void CFItem::setDirEntry(const std::shared_ptr<IDirectoryEntry> &newDirEntry)
{
    if (newDirEntry == nullptr || newDirEntry->getSid() < 0)
        throw new CFException("Attempting to create a CFStorage using an unitialized directory");

    dirEntry = newDirEntry;
}


void CFItem::CheckDisposed() const
{
    if (compoundFile.lock()->IsClosed())
        throw new CFDisposedException("Owner Compound file has been closed and owned items have been invalidated");
}

