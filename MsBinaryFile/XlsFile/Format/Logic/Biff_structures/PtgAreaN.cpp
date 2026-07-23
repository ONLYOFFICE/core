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

#include "PtgAreaN.h"
#include "CellRangeRef.h"

namespace XLS
{

PtgAreaN::PtgAreaN(const unsigned short full_ptg_id, const CellRef cell_base_ref_init) : OperandPtg(full_ptg_id), cell_base_ref(cell_base_ref_init)
{
}

PtgAreaN::PtgAreaN(const std::wstring& word, const PtgDataType data_type, const CellRef cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	area(word),
    areaXlsb(word),
	cell_base_ref(cell_base_ref_init)
{
	area -= cell_base_ref;
    areaXlsb -= cell_base_ref;
}

void PtgAreaN::set_base_ref(const CellRef& cell_base_ref_new)
{
	area += cell_base_ref;
    areaXlsb += cell_base_ref;
	
	cell_base_ref = cell_base_ref_new;
	
	area -= cell_base_ref;
    areaXlsb -= cell_base_ref;
}


BiffStructurePtr PtgAreaN::clone()
{
	return BiffStructurePtr(new PtgAreaN(*this));
}


void PtgAreaN::loadFields(CFRecord& record)
{
    global_info = record.getGlobalWorkbookInfo();

    if (global_info->Version < 0x600)
	{
		unsigned char	colFirst, colLast;
		_UINT16			rwFirst, rwLast;

		record >> rwFirst >> rwLast >> colFirst >> colLast;

		area.rowFirstRelative		= rwFirst & 0x8000;
		area.columnFirstRelative	= rwFirst & 0x4000;
		
		area.columnFirst			= colFirst;
		area.rowFirst				= rwFirst & 0x3FFF;

		area.rowLastRelative	= rwLast & 0x8000;
		area.columnLastRelative	= rwLast & 0x4000;
		
		area.columnLast			= colLast;
		area.rowLast			= rwLast & 0x3FFF;
	}
    else if (global_info->Version < 0x0800)
    {
        record >> area;
    }
    else
    {
        record >> areaXlsb;
    }
}

void PtgAreaN::writeFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	if (record.getGlobalWorkbookInfo()->Version < 0x600)
	{
		unsigned char	colFirst, colLast;
		_UINT16			rwFirst, rwLast;

		rwFirst = (area.rowFirstRelative << 17) & (area.columnFirstRelative << 16) & (area.rowFirst & 0x3FFF);
		colFirst = area.columnFirst;

		rwLast = (area.rowLastRelative << 17) & (area.columnLastRelative << 16) & (area.rowLast & 0x3FFF);
		colLast = area.columnLast;

		record << rwFirst << rwLast << colFirst << colLast;
	}

	else if (global_info->Version < 0x0800)
	{
		record << area;
	}

	else
	{
		record << areaXlsb;
	}

}


void PtgAreaN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
    if (global_info->Version < 0x0800)
    {
        ptg_stack.push((area + cell_base_ref).toString());
    }

    else
    {
        ptg_stack.push((areaXlsb + cell_base_ref).toString(true, true));
    }
}


} // namespace XLS

