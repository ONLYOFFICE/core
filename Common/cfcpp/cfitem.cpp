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
#include "cfitem.h"
#include "idirectoryentry.h"
#include "compoundfile_impl.h"
#include <cstring>


using namespace CFCPP;

_INT32 CFItem::CompareTo(const CFItem &other) const
{
    return dirEntry.lock()->CompareTo(std::dynamic_pointer_cast<RedBlackTree::IRBNode>(other.dirEntry.lock()));
}

bool CFItem::operator==(const CFItem &rightItem) const
{
    return CompareTo(rightItem) == 0;
}

bool CFItem::operator!=(const CFItem &rightItem) const
{
    return CompareTo(rightItem) != 0;
}

_INT32 CFItem::GetHashCode() const
{
    return dirEntry.lock()->GetHashCode();
}

std::wstring CFItem::Name() const
{
    auto name = dirEntry.lock()->GetEntryName();
    if (name.empty() == false)
    {
        auto removeIter = name.find_last_of(L'\0');
        if (removeIter != std::wstring::npos)
            name.erase(removeIter);
        return name;
    }
    else
        return L"";
}

_INT64 CFItem::size() const
{
    return dirEntry.lock()->getSize();
}

bool CFItem::IsStorage() const
{
    return dirEntry.lock()->getStgType() == StgType::StgStorage;
}

bool CFItem::IsStream() const
{
    return dirEntry.lock()->getStgType() == StgType::StgStream;
}

bool CFItem::ISRoot() const
{
    return dirEntry.lock()->getStgType() == StgType::StgRoot;
}

DataTime CFItem::getDataTime() const
{
    if (!dirEntry.use_count())
        return DataTime(0);

    return DataTime(dirEntry.lock()->getCreationDate());
}

void CFItem::setDataTime(const DataTime &value)
{
    if (!dirEntry.use_count())
        return;

    if (dirEntry.lock()->getStgType() != StgStream && dirEntry.lock()->getStgType() != StgRoot)
        dirEntry.lock()->setModifyDate(value.getUINT64());
    else
        throw CFException(L"Modify Date can only be set on storage entries");
}

_GUID_ CFItem::getStorageCLSID() const
{
    return dirEntry.lock()->getStorageCLSID();
}

void CFItem::setStorageCLSID(_GUID_ value)
{
    dirEntry.lock()->setStorageCLSID(value);
}

std::wstring CFItem::ToString() const
{
    if (!dirEntry.expired())
        return L"[" + std::to_wstring(dirEntry.lock()->getLeftSibling()) + L"," + std::to_wstring(dirEntry.lock()->getSid()) + L"," +
                std::to_wstring(dirEntry.lock()->getRightSibling()) + L"] " + dirEntry.lock()->GetEntryName();
    else
        return L"";
}

void CFItem::setDirEntry(const std::weak_ptr<IDirectoryEntry> &newDirEntry)
{
    if (newDirEntry.expired() || newDirEntry.lock()->getSid() < 0)
        throw CFException("Attempting to create a CFStorage using an unitialized directory");

    dirEntry = newDirEntry;
}

std::shared_ptr<IDirectoryEntry> CFItem::getDirEntry() const
{
    return dirEntry.lock();
}


void CFItem::CheckDisposed() const
{
    if (compoundFile != nullptr && compoundFile->IsClosed())
        throw CFDisposedException("Owner Compound file has been closed and owned items have been invalidated");
}


DataTime::DataTime(_UINT64 time)
{
    memcpy(data, reinterpret_cast<char*>(&time), 8);
}

_UINT64 DataTime::getUINT64()const
{
    _UINT64 timeStamp(0);
    memcpy(reinterpret_cast<char*>(&timeStamp), data, 8);

    return timeStamp;
}
