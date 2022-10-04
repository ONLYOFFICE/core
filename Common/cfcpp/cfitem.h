#pragma once
#include "guid.h"
#include "cfexception.h" // Used by heirs
#include <memory>
#include <string>

namespace CFCPP
{
class CompoundFile;
class IDirectoryEntry;


struct DataTime
{
    DataTime(unsigned long long time);
    unsigned long long getUINT64()const;
    char data[8] = {0,0,0,0,0,0,0,0};
};


class CFItem : public std::enable_shared_from_this<CFItem>
{
public:
    int CompareTo(const CFItem& other) const;
    bool operator==(const CFItem &rightItem) const;
    bool operator!=(const CFItem &rightItem) const;
    int GetHashCode() const;
    std::wstring Name() const;
    std::streamsize size() const;
    bool IsStorage() const;
    bool IsStream() const;
    bool ISRoot() const;

    DataTime getDataTime()const;
    void setDataTime(const DataTime& value);

    GUID getStorageCLSID() const;
    void setStorageCLSID(GUID value);

    int CompareTo(const CFItem& other);
    std::wstring ToString() const;

    void setDirEntry(const std::weak_ptr<IDirectoryEntry> &newDirEntry);
    std::shared_ptr<IDirectoryEntry> getDirEntry() const;

    friend class CompoundFile;

protected:
    std::weak_ptr<IDirectoryEntry> dirEntry;
    CompoundFile* compoundFile = nullptr;

protected:
    CFItem() {};
    CFItem(CompoundFile* compoundFile) : compoundFile(compoundFile) {}
    inline CompoundFile* getCompoundFile() {return compoundFile;}
    void CheckDisposed() const;
};
}
