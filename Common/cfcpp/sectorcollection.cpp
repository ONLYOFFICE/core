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
#include "sectorcollection.h"
#include "cfexception.h"


using namespace CFCPP;

SectorCollection::SectorCollection()
{

}

void SectorCollection::Add(std::shared_ptr<Sector> item)
{
    DoCheckSizeLimitReached();

    add(item);
}

void SectorCollection::Clear()
{
    for (auto& slice : largeArraySlices)
        if (slice.isInit())
            slice->clear();

    count = 0;
}

std::shared_ptr<Sector>& SectorCollection::operator[](size_t index)
{
    _INT32 itemIndex = index / SLICE_SIZE;
    _INT32 itemOffset = index % SLICE_SIZE;

    return largeArraySlices[itemIndex][itemOffset];
}

void SectorCollection::DoCheckSizeLimitReached()
{
    if (OnVer3SizeLimitReached.size() && !sizeLimitReached && (count - 1 > MAX_SECTOR_V4_COUNT_LOCK_RANGE))
    {
        sizeLimitReached = true;
        OnVer3SizeLimitReached();
    }
}

_INT32 SectorCollection::add(std::shared_ptr<Sector> item)
{
    _UINT32 itemIndex = count / SLICE_SIZE;

    if (itemIndex < largeArraySlices.size())
    {
        largeArraySlices[itemIndex].push_back(item);
        count++;
    }
    else
    {
        SVector<Sector> ar(SLICE_SIZE);
        ar.push_back(item);
        largeArraySlices.push_back(ar);
        count++;
    }

    return count - 1;
}
