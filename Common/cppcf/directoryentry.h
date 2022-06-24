#pragma once

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

class DirectoryEntry : public IDirectoryEntry
{
public:

    static const int THIS_IS_GREATER = 1;
    static const int OTHER_IS_GREATER = -1;

    static const int NOSTREAM = 0xFFFFFFFF;
    static const int ZERO = 0;

    DirectoryEntry(std::wstring name, StgType stgType, IList<IDirectoryEntry> dirRepository);

    int getSid() const override;
    void setSid(int newSid) override;

    std::wstring GetEntryName() const override;
    void SetEntryName(const std::wstring &entryName) override;
    int GetHashCode() override;
    void Write(Stream stream) override;
    void Read(Stream stream, CFSVersion ver = CFSVersion::Ver_3) override;
    std::wstring ToString() const;
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

private:
    static ULONG64 fnv_hash(char* buffer, int lenght);
private:
    int sid = -1;
    char entryName[64];
    ushort nameLength;
    StgType stgType = StgType::StgInvalid;
    StgColor stgColor = StgColor::Red;


};

}
