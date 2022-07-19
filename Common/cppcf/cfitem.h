#pragma once
#include "compoundfile.h"
#include "guid.h"

namespace CFCPP
{
struct DataTime
{
    // TODO
    char data[8] = {0,0,0,0,0,0,0,0};
};

class CFItem
{
public:
    int CompareTo(const CFItem& other) const;
    bool operator==(const CFItem &rightItem) const;
    bool operator!=(const CFItem &rightItem) const;
    int GetHashCode() const;
    std::wstring Name()const;
    virtual inline std::streamsize size() const {return dirEntry->getSize();}
    bool IsStorage() const;
    bool IsStream() const;
    bool ISRoot() const;

    inline DataTime getDataTime()const{return DataTime();}
    inline void setDataTime(const DataTime& value){};

    GUID getStorageCLSID() const;
    void setStorageCLSID(GUID value);

    int CompareTo(const CFItem& other);
    std::wstring ToString()const;

public:
    std::shared_ptr<IDirectoryEntry> dirEntry;

protected:
    inline CFItem() {};
    inline CFItem(std::shared_ptr<CompoundFile> compoundFile) : compoundFile(compoundFile) {}
    inline std::shared_ptr<CompoundFile> getCompoundFile() {return compoundFile;}
    void CheckDisposed() const;

protected:
    std::shared_ptr<CompoundFile> compoundFile;
};

using PCFItem = std::shared_ptr<CFItem>;


}
