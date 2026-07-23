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

#include "DXFN12.h"

namespace XLS
{


BiffStructurePtr DXFN12::clone()
{
	return BiffStructurePtr(new DXFN12(*this));
}

void DXFN12::load(CFRecord& record)
{
	record >> cbDxf;
	
	if(cbDxf)
	{
		const size_t start_of_struct_pos	= record.getRdPtr();
		const size_t end_of_struct_pos		= record.getRdPtr() + cbDxf;
		
		dxfn = DXFNPtr(new DXFN);
		record >> *dxfn;

		if(record.getRdPtr() < end_of_struct_pos)
		{
			dxfn->xfext = XFExtNoFRTPtr(new XFExtNoFRT);
			record >> *dxfn->xfext;
		}
		record.resetPointerToBegin	(); // file (42).xls (sheet2)
		record.skipNunBytes			(end_of_struct_pos);
	}
	else
	{
		record.skipNunBytes(2); // reserved
	}
}

void DXFN12::save(CFRecord& record)
{
    record.reserveNunBytes(4);
    auto StartDataPose = record.getRdPtr();
    if(dxfn)
    {
		record << *dxfn;

        if(dxfn->xfext != nullptr)
            record << *dxfn->xfext;
        cbDxf = record.getRdPtr() - StartDataPose;
        record.RollRdPtrBack(cbDxf+4);
        record << cbDxf;
        record.skipNunBytes(cbDxf);
    }
    else
        record.reserveNunBytes(2);
}

int DXFN12::serialize(std::wostream & stream)
{
	if (!dxfn) return -1;

	dxfn->serialize(stream);

	return 0;
}

} // namespace XLS

