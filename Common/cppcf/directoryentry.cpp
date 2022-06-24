#include "directoryentry.h"
#include "streamrw.h"

using namespace CFCPP;

DirectoryEntry::DirectoryEntry()
{

}


int DirectoryEntry::getSid() const
{
    return sid;
}

void DirectoryEntry::setSid(int newSid)
{
    sid = newSid;
}

int DirectoryEntry::GetHashCode()
{
    return (int)fnv_hash(entryName, nameLength);
}

void DirectoryEntry::Write(Stream stream)
{
    StreamRW rw(stream);
    rw.WriteArray(entryName, sizeof (entryName));
    rw.Write(nameLength);
    rw.Write((BYTE)stgType);
    rw.Write((BYTE)stgColor);
    rw.Write(leftSibling);
    rw.Write(rightSibling);
    rw.Write(child);
    rw.WriteArray(reinterpret_cast<BYTE*>(&storageCLSID), sizeof (storageCLSID));
    rw.Write(stateBits);
    rw.WriteArray(creationDate, sizeof (creationDate));
    rw.WriteArray(modifyDate, sizeof (modifyDate));
    rw.Write(startSetc);
    rw.Write(size);

    rw.Close();
}

void DirectoryEntry::Read(Stream stream, CFSVersion ver)
{
    StreamRW rw(stream);

    rw.ReadArray(entryName, 64);
    nameLength = rw.Read<decltype(nameLength)>();
    stgType = (StgType)rw.Read<BYTE>();
    stgColor = (StgColor)rw.Read<BYTE>();
    leftSibling = rw.Read<decltype(leftSibling)>();
    rightSibling = rw.Read<decltype(rightSibling)>();
    child = rw.Read<decltype(child)>();

    // Thanks to bugaccount (BugTrack id 3519554)
    if (stgType == StgType::StgInvalid)
    {
        leftSibling = NOSTREAM;
        rightSibling = NOSTREAM;
        child = NOSTREAM;
    }

    rw.ReadArray(reinterpret_cast<char*>(&storageCLSID), 16);
    stateBits = rw.Read<decltype (stateBits)>();
    rw.ReadArray(creationDate, 8);
    rw.ReadArray(modifyDate, 8);
    startSetc = rw.Read<decltype (startSetc)>();

    if (ver == CFSVersion::Ver_3)
    {
        // avoid dirty read for version 3 files (max size: 32bit integer)
        // where most significant bits are not initialized to zero

        size = rw.Read<int>();
        rw.ReadArray<4>(); //discard most significant 4 (possibly) dirty bytes
    }
    else
    {
        size = rw.Read<decltype (size)>();
    }
}

std::wstring DirectoryEntry::ToString() const
{
    std::wstringstream wss;
    wss << Name() << L" [" << sid << L"]" << (stgType == StgType::StgStream ? L"Stream" : L"Storage");
    return wss.str();
}

ULONG64 DirectoryEntry::fnv_hash(char *buffer, int lenght)
{
    ULONG64 h = 2166136261;
    int i;

    for (i = 0; i < lenght; i++)
        h = (h * 16777619) ^ buffer[i];

    return h;
}

