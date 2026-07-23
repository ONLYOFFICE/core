/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
