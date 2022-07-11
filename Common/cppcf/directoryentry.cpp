#include "directoryentry.h"
#include "cfexception.h"
#include "streamrw.h"
#include <stdexcept>


using namespace CFCPP;


DirectoryEntry::DirectoryEntry(std::wstring name, StgType stgType, SVector<IDirectoryEntry> dirRepository)
{
    this->dirRepository = dirRepository;

    this->stgType = stgType;

    if (stgType == StgType::StgStorage)
    {
        //        creationDate = BitConverter.GetBytes((DateTime.Now.ToFileTime()));
        startSetc = ZERO;
    }

    if (stgType == StgType::StgInvalid)
    {
        startSetc = ZERO;
    }

    if (name.size())
    {
        DirectoryEntry::SetEntryName(name);
    }
}

int DirectoryEntry::getSid() const
{
    return sid;
}

void DirectoryEntry::setSid(int newSid)
{
    sid = newSid;
}

std::wstring DirectoryEntry::GetEntryName() const
{
    if (entryName[0] != '\0' && nameLength > 0)
    {
        std::wstring name(entryName, entryName + nameLength);
        return name;
    }
    else
        return L"";
}

void DirectoryEntry::SetEntryName(const std::wstring &entryName)
{
    if (entryName.empty())
    {
        std::fill(this->entryName, this->entryName+64, '\0');
        this->nameLength = 0;
    }
    else
    {
        if (
                entryName.find(L"\\") == std::wstring::npos ||
                entryName.find(L"/")  == std::wstring::npos ||
                entryName.find(L":")  == std::wstring::npos ||
                entryName.find(L"!")  == std::wstring::npos
                ) throw new CFException("Invalid character in entry: the characters '\\', '/', ':','!' cannot be used in entry name");

        if (entryName.length() > 31)
            throw new CFException("Entry name MUST NOT exceed 31 characters");


        std::copy(entryName.data(), entryName.data() + entryName.length(), this->entryName);
        reinterpret_cast<wchar_t*>(this->entryName)[entryName.length()] = L'\0';

        this->nameLength = (ushort)entryName.size() + 2;
    }
}

int DirectoryEntry::GetHashCode() const
{
    return (int)fnv_hash(entryName, nameLength);
}

void DirectoryEntry::Write(Stream stream) const
{
    StreamRW rw(stream);
    rw.WriteArray(entryName, sizeof (entryName));
    rw.Write(nameLength);
    rw.Write((BYTE)stgType);
    rw.Write((BYTE)stgColor);
    rw.Write(leftSibling);
    rw.Write(rightSibling);
    rw.Write(child);
    rw.WriteArray(reinterpret_cast<const char*>(&storageCLSID), sizeof (storageCLSID));
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

RedBlackTree::PIRBNode DirectoryEntry::getLeft() const
{
    if (leftSibling == NOSTREAM)
        return {};

    return dirRepository[leftSibling];
}

RedBlackTree::PIRBNode DirectoryEntry::getRight() const
{
    if (rightSibling == DirectoryEntry::NOSTREAM)
        return {};

    return dirRepository[rightSibling];
}

void DirectoryEntry::setLeft(RedBlackTree::PIRBNode pNode)
{
    leftSibling = pNode != nullptr ? static_cast<IDirectoryEntry*>(pNode.get())->getSid() : DirectoryEntry::NOSTREAM;

    if (leftSibling != DirectoryEntry::NOSTREAM)
        dirRepository[leftSibling]->setParent(shared_from_this());
}

void DirectoryEntry::setRight(RedBlackTree::PIRBNode pNode)
{
    rightSibling = pNode != nullptr ? static_cast<IDirectoryEntry*>(pNode.get())->getSid() : DirectoryEntry::NOSTREAM;

    if (rightSibling != DirectoryEntry::NOSTREAM)
        dirRepository[rightSibling]->setParent(shared_from_this());
}

RedBlackTree::PIRBNode DirectoryEntry::Sibling() const
{
    if (shared_from_this() == getParent()->getLeft())
        return getParent()->getRight();
    else
        return getParent()->getLeft();
}

RedBlackTree::PIRBNode DirectoryEntry::Uncle() const
{
    return parent.use_count() != 0 ? getParent()->Sibling() : RedBlackTree::PIRBNode();
}

void DirectoryEntry::AssignValueTo(RedBlackTree::PIRBNode other)
{
    auto d = std::dynamic_pointer_cast<DirectoryEntry>(other); // as
    if (d == nullptr)
        return;


    d->SetEntryName(this->GetEntryName());

    std::copy(creationDate, creationDate+8, d->creationDate);
    std::copy(modifyDate, modifyDate+8, d->modifyDate);

    d->size = this->size;
    d->startSetc = this->startSetc;
    d->stateBits = this->stateBits;
    d->stgType = this->stgType;
    d->storageCLSID = this->storageCLSID;
    d->child = this->child;
}

int DirectoryEntry::CompareTo(const RedBlackTree::PIRBNode &other) const
{
    IDirectoryEntry* otherDir = dynamic_cast<IDirectoryEntry*>(other.get());

    if (otherDir == nullptr)
        throw new CFException("Invalid casting: compared object does not implement IDirectorEntry interface");

    if (this->getNameLength() > otherDir->getNameLength())
    {
        return THIS_IS_GREATER;
    }
    else if (this->getNameLength() < otherDir->getNameLength())
    {
        return OTHER_IS_GREATER;
    }
    else
    {
        std::wstring thisName = GetEntryName();
        std::wstring otherName = otherDir->GetEntryName();

        for (int z = 0; z < (int)thisName.size(); z++)
        {
            char thisChar = toupper(thisName[z]);
            char otherChar = toupper(otherName[z]);

            if (thisChar > otherChar)
                return THIS_IS_GREATER;
            else if (thisChar < otherChar)
                return OTHER_IS_GREATER;
        }

        return 0;

    }
}

ULONG64 DirectoryEntry::fnv_hash(const char *buffer, int lenght)
{
    ULONG64 h = 2166136261;
    int i;

    for (i = 0; i < lenght; i++)
        h = (h * 16777619) ^ buffer[i];

    return h;
}

std::shared_ptr<IDirectoryEntry> DirectoryEntry::New(std::wstring name, StgType stgType, SVector<IDirectoryEntry> dirRepository)
{
    std::shared_ptr<DirectoryEntry> de;
    if (dirRepository.size())
    {
        de.reset(new DirectoryEntry(name, stgType, dirRepository));
        // No invalid directory entry found
        dirRepository.push_back(de);
        de->setSid(dirRepository.size() - 1);
    }
    else
        throw new std::invalid_argument("dirRepository Directory repository cannot be null in New() method");

    return de;
}

std::shared_ptr<IDirectoryEntry> DirectoryEntry::TryNew(std::wstring name, StgType stgType, SVector<IDirectoryEntry> dirRepository)
{
    std::shared_ptr<DirectoryEntry> de(new DirectoryEntry(name, stgType, dirRepository));

    // If we are not adding an invalid dirEntry as
    // in a normal loading from file (invalid dirs MAY pad a sector)
    if (de != nullptr)
    {
        // Find first available invalid slot (if any) to reuse it
        for (int i = 0; i < (int)dirRepository.size(); i++)
        {
            if (dirRepository[i]->getStgType() == StgType::StgInvalid)
            {
                dirRepository[i] = de;
                de->sid = i;
                return de;
            }
        }
    }

    // No invalid directory entry found
    dirRepository.push_back(de);
    de->sid = dirRepository.size() - 1;

    return de;
}

std::shared_ptr<IDirectoryEntry> DirectoryEntry::Mock(std::wstring name, StgType stgType)
{
    auto de = std::shared_ptr<IDirectoryEntry>(new DirectoryEntry(name, stgType, {}));

    return de;
}

