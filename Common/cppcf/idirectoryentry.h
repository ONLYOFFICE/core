#pragma once

#include <map>
#include "compoundfile.h"
#include "RBTree/irbnode.h"

namespace CFCPP
{
class IDirectoryEntry : public RedBlackTree::IRBNode
{
public:

    virtual int getSid() const = 0;
    virtual void setSid(int newSid) = 0;

    virtual std::wstring GetEntryName() const = 0;
    virtual void SetEntryName(const std::wstring &entryName) = 0;
    virtual ushort getNameLength() const = 0;

    virtual void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) = 0;
    virtual void Write(Stream stream) const = 0;
    virtual int GetHashCode() const = 0;
};
}
