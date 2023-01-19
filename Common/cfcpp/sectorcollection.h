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

#include "sector.h"
#include <list>
#include "svector.h"
#include "event.h"


namespace CFCPP
{
using Ver3SizeLimitReached = std::function<void()>;

class SectorCollection
{
public:
    std::vector<SVector<Sector>> largeArraySlices;
    SectorCollection();
    void Add(std::shared_ptr<Sector> item);
    void Clear();
    inline _INT32 Count()const {return count;}
    std::shared_ptr<Sector>& operator[](size_t index);
    Event<Ver3SizeLimitReached> OnVer3SizeLimitReached;

private:
    void DoCheckSizeLimitReached();
    _INT32 add(std::shared_ptr<Sector> item);

private:
    const _INT32 MAX_SECTOR_V4_COUNT_LOCK_RANGE = 524287; // 0x7FFFFF00 for Version 4
    const _INT32 SLICE_SIZE = 4096;
    bool sizeLimitReached = false;

    _INT32 count = 0;
};
}
