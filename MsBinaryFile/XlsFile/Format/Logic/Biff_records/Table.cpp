/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
        _UINT16 flags;
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

        BYTE flags;
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

void Table::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		R_RwU rwInpRw;
		Col_NegativeOne colInpRw;
		R_RwU rwInpCol;
		Col_NegativeOne colInpCol;

		record << ref_;
		_UINT16 flags = 0;

		SETBIT(flags, 0, fAlwaysCalc)
		SETBIT(flags, 2, fRw)
		SETBIT(flags, 3, fTbl2)
		SETBIT(flags, 4, fDeleted1)
		SETBIT(flags, 5, fDeleted2)

		record << flags;

		CellRef ref1(r1);
		CellRef ref2(r2);

		rwInpRw = ref1.getRow();
		colInpRw = ref1.getColumn();
		rwInpCol = ref2.getRow();
		colInpCol = ref2.getColumn();

		record << rwInpRw << colInpRw << rwInpCol << colInpCol;
	}
	else
	{
		UncheckedRw  rwInput1;
		UncheckedCol colInput1;
		UncheckedRw  rwInput2;
		UncheckedCol colInput2;

		record << rfx;

		CellRef ref1(r1);
		CellRef ref2(r2);

		rwInput1 = ref1.getRow();
		colInput1 = ref1.getColumn();
		rwInput2 = ref2.getRow();
		colInput2 = ref2.getColumn();

		record << rwInput1 << colInput1 << rwInput2 << colInput2;

		BYTE flags = 0;

		SETBIT(flags, 0, fRw)
		SETBIT(flags, 1, fTbl2)
		SETBIT(flags, 2, fDeleted1)
		SETBIT(flags, 3, fDeleted2)
		SETBIT(flags, 4, fAlwaysCalc)

		record << flags;
	}
}

} // namespace XLS

