#include "cfitem.h"
#include "idirectoryentry.h"
#include "compoundfile.h"
#include <cstring>

using namespace CFCPP;


int CFItem::CompareTo(const CFItem &other) const
{
    return dirEntry.lock()->CompareTo(std::dynamic_pointer_cast<RedBlackTree::IRBNode>(other.dirEntry.lock()));
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
    return dirEntry.lock()->GetHashCode();
}

std::wstring CFItem::Name() const
{
    auto n = dirEntry.lock()->GetEntryName();
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

std::streamsize CFItem::size() const
{
    return dirEntry.lock()->getSize();
}

bool CFItem::IsStorage() const
{
    return dirEntry.lock()->getStgType() == StgType::StgStorage;
}

bool CFItem::IsStream() const
{
    return dirEntry.lock()->getStgType() == StgType::StgStream;
}

bool CFItem::ISRoot() const
{
    return dirEntry.lock()->getStgType() == StgType::StgRoot;
}

DataTime CFItem::getDataTime() const
{
    if (!dirEntry.use_count())
        return DataTime(0);

    return DataTime(dirEntry.lock()->getCreationDate());
}

void CFItem::setDataTime(const DataTime &value)
{

}

GUID CFItem::getStorageCLSID() const
{
    return dirEntry.lock()->getStorageCLSID();
}

void CFItem::setStorageCLSID(GUID value)
{
    dirEntry.lock()->setStorageCLSID(value);
}

std::wstring CFItem::ToString() const
{
    if (!dirEntry.expired())
        return L"[" + std::to_wstring(dirEntry.lock()->getLeftSibling()) + L"," + std::to_wstring(dirEntry.lock()->getSid()) + L"," +
                std::to_wstring(dirEntry.lock()->getRightSibling()) + L"] " + dirEntry.lock()->GetEntryName();
    else
        return L"";
}

void CFItem::setDirEntry(const std::weak_ptr<IDirectoryEntry> &newDirEntry)
{
    if (newDirEntry.expired() || newDirEntry.lock()->getSid() < 0)
        throw CFException("Attempting to create a CFStorage using an unitialized directory");

    dirEntry = newDirEntry;
}

std::shared_ptr<IDirectoryEntry> CFItem::getDirEntry() const
{
    return dirEntry.lock();
}


void CFItem::CheckDisposed() const
{
    if (compoundFile != nullptr && compoundFile->IsClosed())
        throw CFDisposedException("Owner Compound file has been closed and owned items have been invalidated");
}


DataTime::DataTime(unsigned long long time)
{
    memcpy(data, reinterpret_cast<char*>(&time), 8);
}

unsigned long long DataTime::getUINT64()const
{
    unsigned long long t(0);
    memcpy(reinterpret_cast<char*>(&t), data, 8);

    return t;
}
