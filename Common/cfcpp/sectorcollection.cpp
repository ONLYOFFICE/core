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
