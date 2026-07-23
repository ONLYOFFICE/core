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

#include <memory>
#include <vector>
#include <mutex>
#include "Stream/stream.h"


namespace CFCPP
{

enum SectorType
{
    Normal,
    Mini,
    FAT,
    DIFAT,
    RangeLockSector,
    Directory
};

class Sector
{
public:
    Sector(_INT32 size, const Stream stream);
    Sector(_INT32 size, const std::vector<BYTE> &data);
    Sector(_INT32 size);

    bool IsStreamed();
    void ZeroData();
    void InitFATData();
    void ReleaseData();

    virtual void Dispose(bool disposing=false);
    std::vector<BYTE> &GetData();

public:
    static _INT32 MINISECTOR_SIZE;
    const static _INT32 FREESECT = 0xFFFFFFFF;
    const static _INT32 ENDOFCHAIN = 0xFFFFFFFE;
    const static _INT32 FATSECT = 0xFFFFFFFD;
    const static _INT32 DIFSECT = 0xFFFFFFFC;

    _INT32 getSize() const;

    SectorType type = Normal;
    bool dirtyFlag = false;
    _INT32 id = -1;

private:
    _INT32 size = 0;
    Stream stream;
    std::vector<BYTE> data;
    std::mutex lockObject;
    bool _disposed = false;
};

}
