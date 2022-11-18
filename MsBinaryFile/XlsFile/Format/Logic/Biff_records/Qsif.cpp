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

#include "Qsif.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/XLWideString.h"

namespace XLS
{

Qsif::Qsif()
{
}

Qsif::~Qsif()
{
}

BaseObjectPtr Qsif::clone()
{
	return BaseObjectPtr(new Qsif(*this));
}

void Qsif::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        unsigned short  flags1, flags2;
        XLUnicodeString rgbTitle;

        record >> frtHeaderOld >> flags1 >> flags2 >> idField;

        fUserIns	= GETBIT(flags1, 0);
        fFillDown	= GETBIT(flags1, 1);
        fSortDes	= GETBIT(flags1, 2);
        iSortKey	= GETBITS(flags1, 3, 10);
        fRowNums	= GETBIT(flags1, 11);
        fSorted		= GETBIT(flags1, 13);

        fClipped	= GETBIT(flags2, 0);

        if (record.getRdPtr() >= record.getDataSize())
            return;

        record >> idList >> rgbTitle;

        name        = rgbTitle.value();
    }

    else
    {
        _UINT32            flags;
        XLSB::XLWideString irstName;
        record >> flags >> idField >> idList;

        fUserIns	= GETBIT(flags, 0);
        fFillDown	= GETBIT(flags, 1);
        fRowNums	= GETBIT(flags, 2);
        fClipped	= GETBIT(flags, 3);
        fIrstName	= GETBIT(flags, 4);

        if(fIrstName)
        {
            record >> irstName;
            name    = irstName.value();
        }
    }
}

} // namespace XLS

