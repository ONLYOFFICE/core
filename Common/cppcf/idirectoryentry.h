#pragma once

#include <map>
#include "compoundfile.h"
#include "RBTree/irbnode.h"

namespace CFCPP
{
class IDirectoryEntry : public RedBlackTree::IRBNode
{
public:

    virtual int getChild() const = 0;
    virtual void setChild(int value) = 0;
    virtual int getLeftSibling() const = 0;
    virtual void setLeftSibling(int value) = 0;
    virtual int getRightSibling() const = 0;
    virtual void setRightSibling(int value) = 0;

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
    virtual int GetHashCode() const = 0;
};
}
