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

#include "Table.h"

namespace XLS
{

Table::Table()
{
}


Table::~Table()
{
}


BaseObjectPtr Table::clone()
{
	return BaseObjectPtr(new Table(*this));
}

void Table::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        R_RwU rwInpRw;
        Col_NegativeOne colInpRw;
        R_RwU rwInpCol;
        Col_NegativeOne colInpCol;

        record >> ref_;
        unsigned short flags;
        record >> flags;
        fAlwaysCalc     = GETBIT(flags, 0);
        fRw             = GETBIT(flags, 2);
        fTbl2           = GETBIT(flags, 3);
        fDeleted1       = GETBIT(flags, 4);
        fDeleted2       = GETBIT(flags, 5);

        record >> rwInpRw >> colInpRw >> rwInpCol >> colInpCol;
        r1 = static_cast<std::wstring >(CellRef(rwInpRw, colInpRw, true, true));
        if(fTbl2)
        {
            r2 = static_cast<std::wstring >(CellRef(rwInpCol, colInpCol, true, true));
        }
    }
    else
    {
        UncheckedRw  rwInput1;
        UncheckedCol colInput1;
        UncheckedRw  rwInput2;
        UncheckedCol colInput2;

        record >> rfx;
        record >> rwInput1 >> colInput1 >> rwInput2 >> colInput2;

        unsigned char flags;
        record >> flags;
        fRw             = GETBIT(flags, 0);
        fTbl2           = GETBIT(flags, 1);
        fDeleted1       = GETBIT(flags, 2);
        fDeleted2       = GETBIT(flags, 3);
        fAlwaysCalc     = GETBIT(flags, 4);

        if(fDeleted1)
        {
            rwInput1 = colInput1 = 0;
        }
        if(fDeleted2 || !fTbl2)
        {
            rwInput2 = colInput2 = 0;
        }

        r1 = static_cast<std::wstring >(CellRef(rwInput1, colInput1, true, true));
        r2 = static_cast<std::wstring >(CellRef(rwInput2, colInput2, true, true));
    }
}

} // namespace XLS

