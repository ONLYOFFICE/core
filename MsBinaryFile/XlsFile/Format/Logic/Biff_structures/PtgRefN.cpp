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

#include "PtgRefN.h"
#include "CellRef.h"

namespace XLS
{

PtgRefN::PtgRefN(const unsigned short full_ptg_id, const CellRef& cell_base_ref_init) : OperandPtg(full_ptg_id), cell_base_ref(cell_base_ref_init)
{
	bUseLocInit = false;
}

PtgRefN::PtgRefN(const std::wstring& word, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
    loc(word),
    loc_xlsb(word),
	cell_base_ref(cell_base_ref_init)
{
	loc -= cell_base_ref;
    loc_xlsb -= cell_base_ref;
	bUseLocInit = true;
}

void PtgRefN::set_base_ref(const CellRef& cell_base_ref_new)
{
    if (bUseLocInit)
    {
        loc += cell_base_ref;
        loc_xlsb += cell_base_ref;
    }
	
	cell_base_ref = cell_base_ref_new;
	
    if (bUseLocInit)
    {
        loc -= cell_base_ref;
        loc_xlsb -= cell_base_ref;
    }
}

BiffStructurePtr PtgRefN::clone()
{
	return BiffStructurePtr(new PtgRefN(*this));
}

void PtgRefN::loadFields(CFRecord& record)
{
    global_info = record.getGlobalWorkbookInfo();
    if (global_info->Version < 0x0600)
	{
		unsigned char	col;
		_UINT16			rw;
		record >> rw >> col;

		loc.rowRelative	= rw & 0x8000;
		loc.colRelative	= rw & 0x4000;

		loc.column	= col;
		loc.row		= GETBITS(rw, 0, 13);
	}
	else if (global_info->Version < 0x0800)
    {
       record >> loc;
    }
    else
    {
       record >> loc_xlsb;
    }

}
void PtgRefN::writeFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	if (global_info->Version < 0x0600)
	{
		unsigned char	col = 0;
		_UINT16			rw = 0;

		SETBIT(rw, 15, loc.rowRelative)
		SETBIT(rw, 14, loc.colRelative)
		SETBITS(rw, 0, 13, loc.row)

		col = loc.column;

		record << rw << col;
	}
	else if (global_info->Version < 0x0800)
	{
		record << loc;
	}
	else
	{
		record << loc_xlsb;
	}
}

void PtgRefN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
    if (global_info->Version < 0x0800)
    {
       ptg_stack.push((loc + cell_base_ref).toString());
    }
    else
    {
       ptg_stack.push((loc_xlsb + cell_base_ref).toString(true));
    }
}



} // namespace XLS

