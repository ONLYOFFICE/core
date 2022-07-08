#pragma once

#include <map>
#include "Stream.h"
#include "RBTree/irbnode.h"
#include "guid.h"

namespace CFCPP
{
enum StgType : int
{
    StgInvalid = 0,
    StgStorage = 1,
    StgStream = 2,
    StgLockbytes = 3,
    StgProperty = 4,
    StgRoot = 5
};

enum StgColor : int
{
    Red = 0,
    Black = 1
};

enum CFSVersion : int
{
    /// Compound file version 3 - The default and most common version available. Sector size 512 bytes, 2GB max file size.
    Ver_3 = 3,
    /// Compound file version 4 - Sector size is 4096 bytes. Using this version could bring some compatibility problem with existing applications.
    Ver_4 = 4
};

class IDirectoryEntry : public RedBlackTree::IRBNode
{
public:

    virtual int getChild() const = 0;
    virtual void setChild(int value) = 0;
    virtual int getLeftSibling() const = 0;
    virtual void setLeftSibling(int value) = 0;
    virtual int getRightSibling() const = 0;
    virtual void setRightSibling(int value) = 0;
    virtual std::streamsize getSize() const = 0;
    virtual void setSize(std::streamsize value) = 0;

    virtual std::array<BYTE,8> getCreationDate() const = 0;
    virtual void setCreationDate(const std::array<BYTE,8>& date) = 0;

    virtual int getSid() const = 0;
    virtual void setSid(int newSid) = 0;

    virtual std::wstring GetEntryName() const = 0;
    virtual void SetEntryName(const std::wstring &entryName) = 0;
    virtual ushort getNameLength() const = 0;

    virtual void setStartSetc(int value) = 0;
    virtual int getStartSetc() const = 0;

    virtual void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) = 0;
    virtual void Write(Stream stream) const = 0;
    virtual StgType getStgType() const = 0;
    virtual void setStgType(StgType value) = 0;
    virtual GUID getStorageCLSID() const = 0;
    virtual void setStorageCLSID(GUID value) = 0;
    virtual int GetHashCode() const = 0;
};
}
