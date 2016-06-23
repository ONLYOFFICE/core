/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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


void Table::writeFields(CFRecord& record)
{
	R_RwU rwInpRw;
	Col_NegativeOne colInpRw;
	R_RwU rwInpCol;
	Col_NegativeOne colInpCol;

	record << ref_;
	unsigned short flags = 0;
	SETBIT(flags, 0, fAlwaysCalc);
	SETBIT(flags, 2, fRw);
	SETBIT(flags, 3, fTbl2);
	SETBIT(flags, 4, fDeleted1);
	SETBIT(flags, 5, fDeleted2);
	record << flags;

	if(!fDeleted1)
	{
		CellRef r1_ref(static_cast<std::wstring >(r1));
		rwInpRw = static_cast<unsigned short>(r1_ref.getRow());
		colInpRw = static_cast<short>(r1_ref.getColumn());
	}
	else
	{
		rwInpRw = 65535;
		colInpRw = -1;
	}
	record << rwInpRw << colInpRw;
	if(fTbl2 && !fDeleted1)
	{
		CellRef r2_ref(static_cast<std::wstring >(r2));
		rwInpCol = static_cast<unsigned short>(r2_ref.getRow());
		colInpCol = static_cast<short>(r2_ref.getColumn());
	}
	else
	{
		rwInpCol = 65535;
		colInpCol = -1;
	}
	record << rwInpCol << colInpCol;
}


void Table::readFields(CFRecord& record)
{
	R_RwU rwInpRw;
	Col_NegativeOne colInpRw;
	R_RwU rwInpCol;
	Col_NegativeOne colInpCol;

	record >> ref_;
	unsigned short flags;
	record >> flags;
	fAlwaysCalc = GETBIT(flags, 0);
	fRw = GETBIT(flags, 2);
	fTbl2 = GETBIT(flags, 3);
	fDeleted1 = GETBIT(flags, 4);
	fDeleted2 = GETBIT(flags, 5);

	record >> rwInpRw >> colInpRw >> rwInpCol >> colInpCol;
	r1 = static_cast<std::wstring >(CellRef(rwInpRw, colInpRw, true, true));
	if(fTbl2)
	{
		r2 = static_cast<std::wstring >(CellRef(rwInpCol, colInpCol, true, true));
	}
}

} // namespace XLS

