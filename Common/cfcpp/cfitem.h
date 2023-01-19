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
#pragma once

#include "guid.h"
#include "cfexception.h" // Used by heirs
#include <memory>
#include <string>

namespace CFCPP
{
struct DataTime
{
    DataTime(_UINT64 time);
    _UINT64 getUINT64()const;
    char data[8] = {0,0,0,0,0,0,0,0};
};

class CompoundFile_impl;
class IDirectoryEntry;

class CFItem : public std::enable_shared_from_this<CFItem>
{
public:
    _INT32 CompareTo(const CFItem& other) const;
    bool operator==(const CFItem &rightItem) const;
    bool operator!=(const CFItem &rightItem) const;
    _INT32 GetHashCode() const;
    std::wstring Name() const;
    _INT64 size() const;
    bool IsStorage() const;
    bool IsStream() const;
    bool ISRoot() const;

    DataTime getDataTime()const;
    void setDataTime(const DataTime& value);

	_GUID_ getStorageCLSID() const;
    void setStorageCLSID(_GUID_ value);

    _INT32 CompareTo(const CFItem& other);
    std::wstring ToString() const;

    void setDirEntry(const std::weak_ptr<IDirectoryEntry> &newDirEntry);
    std::shared_ptr<IDirectoryEntry> getDirEntry() const;

    friend class CompoundFile_impl;

protected:
    std::weak_ptr<IDirectoryEntry> dirEntry;
    CompoundFile_impl* compoundFile = nullptr;

protected:
    CFItem() {};
    CFItem(CompoundFile_impl* compoundFile) :
        compoundFile(compoundFile)
    {}
    inline CompoundFile_impl* getCompoundFile()
        {return compoundFile;}
    void CheckDisposed() const;
};
}
