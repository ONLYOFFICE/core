﻿/*
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
//
#include "Array.h"

namespace XLS
{

Array::Array(const CellRef& cell_base_ref_init) :formula(false, cell_base_ref_init)
{
}

Array::~Array()
{
}


BaseObjectPtr Array::clone()
{
	return BaseObjectPtr(new Array(*this));
}

void Array::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> ref_;
        _UINT16 flags;
        record >> flags;
        fAlwaysCalc = GETBIT(flags, 0);
        record.skipNunBytes(4); // unused
        formula.load(record);
    }
    else
    {
        record >> rfx;
        BYTE flags;
        record >> flags;
        fAlwaysCalc = GETBIT(flags, 0);
        formula.load(record);
    }
}

void Array::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		record << ref_;
		_UINT16 flags = 0;
		SETBIT(flags, 0, fAlwaysCalc);
		record << flags;
		record.reserveNunBytes(4); // unused
		formula.save(record);
	}
	else
	{
		record << rfx;
		BYTE flags = 0;
		SETBIT(flags, 0, fAlwaysCalc);
		record << flags;
		formula.save(record);
	}
}


} // namespace XLS

