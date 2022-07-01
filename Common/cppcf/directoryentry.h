#pragma once

#include "svector.h"
#include "idirectoryentry.h"
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

class DirectoryEntry : public IDirectoryEntry, protected std::enable_shared_from_this<DirectoryEntry>
{
public:

    static const int THIS_IS_GREATER = 1;
    static const int OTHER_IS_GREATER = -1;

    static const int NOSTREAM = 0xFFFFFFFF;
    static const int ZERO = 0;

    DirectoryEntry(std::wstring name, StgType stgType, SVector<IDirectoryEntry> dirRepository);

    RedBlackTree::PIRBNode getLeft() const override;
    RedBlackTree::PIRBNode getRight() const override;
    void setLeft(RedBlackTree::PIRBNode pNode) override;
    void setRight(RedBlackTree::PIRBNode pNode) override;

    inline void setColor(RedBlackTree::Color clr) override {stgColor = (StgColor)clr;}
    inline RedBlackTree::Color getColor()const override {return (RedBlackTree::Color)stgColor;}

    void setParent(RedBlackTree::PIRBNode pParent) override {parent = pParent;}
    inline RedBlackTree::PIRBNode getParent() const override {return parent.lock();}
    inline RedBlackTree::PIRBNode Grandparent() const override
        {return (parent.use_count() ? parent.lock()->getParent() : RedBlackTree::PIRBNode());}
    RedBlackTree::PIRBNode Sibling() const override; // check parent before using
    RedBlackTree::PIRBNode Uncle() const override;
    void AssignValueTo(RedBlackTree::PIRBNode other) override;

    int CompareTo(const RedBlackTree::PIRBNode& other) const override;
    std::wstring ToString() const override;

    int getSid() const override;
    void setSid(int newSid) override;

    std::wstring GetEntryName() const override;
    void SetEntryName(const std::wstring &entryName) override;
    inline ushort getNameLength() const override {return  nameLength;}

    void setStartSetc(int value) override {startSetc = value;};
    int getStartSetc() const override {return startSetc;};

    void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) override;
    void Write(Stream stream) const override;
    int GetHashCode() const override;

    StgType getStgType() const;
    inline std::wstring Name() const {return GetEntryName();}

public:
    BYTE creationDate[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    BYTE modifyDate[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int startSetc = Sector::ENDOFCHAIN;
    LONG64 size;
    int leftSibling = NOSTREAM;
    int rightSibling = NOSTREAM;
    int child = NOSTREAM;
    GUID storageCLSID;
    int stateBits;
    static std::shared_ptr<IDirectoryEntry> New(std::wstring name, StgType stgType, SVector<IDirectoryEntry> dirRepository);

private:
    static ULONG64 fnv_hash(const char *buffer, int lenght);

private:
    int sid = -1;
    char entryName[64];
    ushort nameLength;
    StgType stgType = StgType::StgInvalid;
    StgColor stgColor = StgColor::Red;
    SVector<IDirectoryEntry> dirRepository;
    std::weak_ptr<RedBlackTree::IRBNode> parent;

};

}
