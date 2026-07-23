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

#include "cfstorage.h"
#include "idirectoryentry.h"

namespace CFCPP
{

enum CFSConfiguration
{
    Default = 1,                // No other flags
    SectorRecycle = 2,          // Rewrite unused sectors
    EraseFreeSectors = 4,       // Free sectors are erased to avoid information leakage
    NoValidationException = 8,  // Ignore some file reading errors to read broken files
    LeaveOpen = 16              // file will not be closed
};

enum CFSUpdateMode
{
    ReadOnly,
    Update
};

class CompoundFile_impl;
class CompoundFile
{
public:
    CompoundFile(const std::wstring &fileName, CFSUpdateMode updateMode, int configParameters = Default);
    CompoundFile(CFSVersion cfsVersion, CFSConfiguration configFlags);
    CompoundFile(const std::wstring &fileName);
    CompoundFile(Stream stream);
    CompoundFile();

    std::shared_ptr<CFStorage> RootStorage();

    bool Save(std::wstring wFileName);
    void Save(Stream stream);

    void Commit(bool releaseMemory = false);
    bool HasSourceStream() const;
    bool ValidationExceptionEnabled() const;
    bool IsClosed()const;
    void Close();

    std::vector<BYTE> GetDataBySID(_INT32 sid);
    _GUID_ getGuidBySID(_INT32 sid);
    _GUID_ getGuidForStream(_INT32 sid);

private:
    std::shared_ptr<CompoundFile_impl> _impl;
};
}
