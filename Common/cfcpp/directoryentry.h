#pragma once

#include "svector.h"
#include "idirectoryentry.h"

namespace CFCPP
{
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
    std::streamsize getSize() const override {return size;}
    void setSize(std::streamsize value) override {size = value;}
    int getStateBits() const override {return stateBits;}
    void setStateBits(int value) override {stateBits = value;}

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

    inline int getChild() const override {return child;}
    inline void setChild(int value) override {child = value;}

    inline int getLeftSibling() const override {return leftSibling;}
    inline void setLeftSibling(int value) override {leftSibling = value;}
    inline int getRightSibling() const override {return rightSibling;}
    inline void setRightSibling(int value) override {rightSibling = value;}

    inline UINT64 getCreationDate() const override {return creationDate;}
    inline void setCreationDate(const UINT64& value) override {creationDate = value;}
    inline UINT64 getModifyDate() const override {return modifyDate;}
    inline void setModifyDate(const UINT64& value) override {modifyDate = value;}

    int getSid() const override;
    void setSid(int newSid) override;

    std::wstring GetEntryName() const override;
    void SetEntryName(const std::wstring &entryName) override;
    inline ushort getNameLength() const override {return  nameLength;}

    void setStartSetc(int value) override {startSetc = value;};
    int getStartSetc() const override {return startSetc;};

    void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) override;
    void Write(Stream stream) const override;
    inline StgColor getStgColor() const override {return stgColor;}
    inline void setStgColor(StgColor value) override {stgColor = value;}
    inline StgType getStgType() const override {return stgType;}
    inline void setStgType(StgType value) override {stgType = value;}
    inline GUID getStorageCLSID() const override {return storageCLSID;}
    inline void setStorageCLSID(GUID value) override {storageCLSID = value;}
    int GetHashCode() const override;

    inline std::wstring Name() const {return GetEntryName();}

public:
    UINT64 creationDate = 0;
    UINT64 modifyDate = 0;
    int startSetc = 0xFFFFFFFE;
    LONG64 size;
    int leftSibling = NOSTREAM;
    int rightSibling = NOSTREAM;
    int child = NOSTREAM;
    int stateBits;
    static std::shared_ptr<IDirectoryEntry> New(std::wstring name, StgType stgType, SVector<IDirectoryEntry>& dirRepository);
    static std::shared_ptr<IDirectoryEntry> TryNew(std::wstring name, StgType stgType, SVector<IDirectoryEntry> &dirRepository);
    static std::shared_ptr<IDirectoryEntry> Mock(std::wstring name, StgType stgType);

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
    GUID storageCLSID;

};

}
