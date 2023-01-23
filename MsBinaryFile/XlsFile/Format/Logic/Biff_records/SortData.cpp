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

#include "SortData.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/CellRangeRef.h"

namespace XLS
{

SortData::SortData()
{
}

SortData::~SortData()
{
}

BaseObjectPtr SortData::clone()
{
	return BaseObjectPtr(new SortData(*this));
}

void SortData::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        unsigned short flags;

        record >> frtHeader >> flags;

        fCol			= GETBIT(flags, 0);
        fCaseSensitive	= GETBIT(flags, 1);
        fAltMethod		= GETBIT(flags, 2);
        sfp				= GETBITS(flags, 3, 5);

        RFX rfx_orig;
        record >> rfx_orig >> cconditions;
        rfx = static_cast<std::wstring >(rfx_orig);

        record >> idParent;

        std::list<CFRecordPtr>& recs = continue_records[rt_ContinueFrt12];
        while(!recs.empty())
        {
            record.appendRawData(recs.front()->getData() + 12, recs.front()->getDataSize() - 12);
            recs.pop_front();
        }

        for(unsigned int i = 0; i < cconditions; ++i)
        {
            SortCond12Ptr sort_cond(new SortCond12);
            record >> *sort_cond;
            sortCond12Array.push_back(sort_cond);
        }
    }
    else
    {
        unsigned short flags;

        record >> flags;

        fCol			= GETBIT(flags, 0);
        fCaseSensitive	= GETBIT(flags, 1);
        fAltMethod		= GETBIT(flags, 2);

        XLSB::UncheckedRfX rfx_orig;
        record >> rfx_orig;
        rfx = static_cast<std::wstring >(rfx_orig);

    }

}

} // namespace XLS

