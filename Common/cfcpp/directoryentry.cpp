/*
* (c) Copyright Ascensio System SIA 2010-2019
*
* This program is a free software product. You can redistribute it and/or
* modify it under the terms of the GNU Affero General Public License (AGPL)
* version 3 as published by the Free Software Foundation. In accordance with
* Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
* that Ascensio System SIA expressly excludes the warranty of non-infringement
* of any third-party rights.
*
* This program is distributed WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
* details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
*
* You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
* street, Riga, Latvia, EU, LV-1050.
*
* The  interactive user interfaces in modified source and object code versions
* of the Program must display Appropriate Legal Notices, as required under
* Section 5 of the GNU AGPL version 3.
*
* Pursuant to Section 7(b) of the License you must retain the original Product
* logo when distributing the program. Pursuant to Section 7(e) we decline to
* grant you any rights under trademark law for use of our trademarks.
*
* All the Product's GUI elements, including illustrations and icon sets, as
* well as technical writing content are licensed under the terms of the
* Creative Commons Attribution-ShareAlike 4.0 International. See the License
* terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
*
*/
#include "directoryentry.h"
#include <sstream>
#include "cfexception.h"
#include "streamrw.h"
#include <stdexcept>
#include <ctime>

using namespace CFCPP;

DirectoryEntry::DirectoryEntry(std::wstring name, StgType stgType, SVector<IDirectoryEntry> &dirRepository) :
    dirRepository(dirRepository)
{
    this->stgType = stgType;

    if (stgType == StgType::StgStorage)
    {
        creationDate = 10000000ULL * std::time(NULL) + 116444736000000000ULL;
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

DirectoryEntry::DirectoryEntry(std::wstring name, StgType stgType) :
    dirRepository(emptyDir)
{
    this->stgType = stgType;

    if (stgType == StgType::StgStorage)
    {
        // file timestamp
        creationDate = 10000000ULL * std::time(NULL) + 116444736000000000ULL;
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

_INT32 DirectoryEntry::getSid() const
{
    return sid;
}

void DirectoryEntry::setSid(_INT32 newSid)
{
    sid = newSid;
}

std::wstring DirectoryEntry::GetEntryName() const
{
    if (entryName[0] != '\0' && nameLength > 0)
    {
        wchar_t name[32];
        for (_INT32 i = 0; i < 32; i++)
        {
            name[i] = entryName[2*i] + (entryName[2*i+1] << 8);
        }
        return std::wstring (name, name + nameLength/2 - 1);

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
                entryName.find(L"\\") != std::wstring::npos ||
                entryName.find(L"/")  != std::wstring::npos ||
                entryName.find(L":")  != std::wstring::npos ||
                entryName.find(L"!")  != std::wstring::npos
                ) throw CFException("Invalid character in entry: the characters '\\', '/', ':','!' cannot be used in entry name");

        if (entryName.length() > 31)
            throw CFException("Entry name MUST NOT exceed 31 characters");


        std::fill(this->entryName, this->entryName + 64, 0);
        for (size_t i = 0; i < entryName.size(); i++)
        {
            wchar_t sym = entryName[i];
            this->entryName[i*2+0] = sym % 256;
            this->entryName[i*2+1] = sym / 256;
        }

        this->nameLength = (_UINT16)entryName.size() * 2 + 2;
    }
}

_INT32 DirectoryEntry::GetHashCode() const
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
    rw.WriteArray(reinterpret_cast<const char*>(&creationDate), sizeof (creationDate));
    rw.WriteArray(reinterpret_cast<const char*>(&modifyDate), sizeof (modifyDate));
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

    if (stgType == StgType::StgInvalid)
    {
        leftSibling = NOSTREAM;
        rightSibling = NOSTREAM;
        child = NOSTREAM;
    }

    rw.ReadArray(reinterpret_cast<char*>(&storageCLSID), 16);
    stateBits = rw.Read<decltype (stateBits)>();
    rw.ReadArray(reinterpret_cast<char*>(&creationDate), 8);
    rw.ReadArray(reinterpret_cast<char*>(&modifyDate), 8);
    startSetc = rw.Read<decltype (startSetc)>();

    if (ver == CFSVersion::Ver_3)
    {
        size = rw.Read<_INT32> ();
        rw.Read<_INT32>();
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
    leftSibling = pNode != nullptr ? std::static_pointer_cast<IDirectoryEntry>(pNode)->getSid() : DirectoryEntry::NOSTREAM;

    if (leftSibling != DirectoryEntry::NOSTREAM)
        dirRepository[leftSibling]->setParent(shared_from_this());
}

void DirectoryEntry::setRight(RedBlackTree::PIRBNode pNode)
{
    rightSibling = pNode != nullptr ? std::static_pointer_cast<IDirectoryEntry>(pNode)->getSid() : DirectoryEntry::NOSTREAM;

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
    auto d = std::dynamic_pointer_cast<DirectoryEntry>(other);
    if (d == nullptr)
        return;


    d->SetEntryName(this->GetEntryName());

    d->creationDate = creationDate;
    d->modifyDate = modifyDate;

    d->size = this->size;
    d->startSetc = this->startSetc;
    d->stateBits = this->stateBits;
    d->stgType = this->stgType;
    d->storageCLSID = this->storageCLSID;
    d->child = this->child;
}

_INT32 DirectoryEntry::CompareTo(const RedBlackTree::PIRBNode &other) const
{
    IDirectoryEntry* otherDir = dynamic_cast<IDirectoryEntry*>(other.get());

    if (otherDir == nullptr)
        throw CFException("Invalid casting: compared object does not implement IDirectorEntry interface");

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

        for (_INT32 z = 0; z < (int)thisName.size(); z++)
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

_UINT64 DirectoryEntry::fnv_hash(const char *buffer, _INT64 lenght)
{
    _UINT64 h = 2166136261;
    _INT64 i;

    for (i = 0; i < lenght; i++)
        h = (h * 16777619) ^ buffer[i];

    return h;
}

std::shared_ptr<IDirectoryEntry> DirectoryEntry::New(std::wstring name, StgType stgType, SVector<IDirectoryEntry>& dirRepository)
{
    std::shared_ptr<IDirectoryEntry> de;
    if (dirRepository)
    {
        de.reset(new DirectoryEntry(name, stgType, dirRepository));
        dirRepository.push_back(de);
        de->setSid(dirRepository.size() - 1);
    }
    else
        throw std::invalid_argument("dirRepository Directory repository cannot be null in New() method");

    return de;
}

std::shared_ptr<IDirectoryEntry> DirectoryEntry::TryNew(std::wstring name, StgType stgType, SVector<IDirectoryEntry>& dirRepository)
{
    std::shared_ptr<DirectoryEntry> de(new DirectoryEntry(name, stgType, dirRepository));

    if (de != nullptr)
    {
        for (_INT32 i = 0; i < (int)dirRepository.size(); i++)
        {
            if (dirRepository[i]->getStgType() == StgType::StgInvalid)
            {
                dirRepository[i] = de;
                de->sid = i;
                return de;
            }
        }
    }

    dirRepository.push_back(de);
    de->sid = dirRepository.size() - 1;

    return de;
}

std::shared_ptr<IDirectoryEntry> DirectoryEntry::Mock(std::wstring name, StgType stgType)
{
    auto de = std::shared_ptr<IDirectoryEntry>(new DirectoryEntry(name, stgType));

    return de;
}

