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

#include "Cell.h"
#include "CellRef.h"

namespace XLS
{


Cell::Cell()
{
}


Cell::Cell(const unsigned short rw_init, const unsigned short col_init, const unsigned short ixfe_init)
{
	rw = rw_init;
	col = col_init;
	ixfe = ixfe_init;
}


BiffStructurePtr Cell::clone()
{
	return BiffStructurePtr(new Cell(*this));
}

void Cell::load(CFRecord& record)
{
	record >> rw >> col;

	if (record.getGlobalWorkbookInfo()->Version == 0x0200)
	{
		unsigned char flags1, flags2, flags3;

		record >> flags1 >> flags2 >> flags3;

		ixfe = GETBITS(flags1, 0, 5);
		
		bool fLocked = GETBIT(flags1, 6);
		bool fHidden = GETBIT(flags1, 7);

		short ifmt_index = GETBITS(flags2, 0, 5);
		short ifnt_index = GETBITS(flags2, 6, 7);		

		short alc = GETBITS(flags3, 0, 2);

		short border_dgLeft = GETBIT(flags3, 3) ? 1 : 0;
		short border_dgRight = GETBIT(flags3, 4) ? 1 : 0;
		short border_dgTop = GETBIT(flags3, 5) ? 1 : 0;
		short border_dgBottom = GETBIT(flags3, 6) ? 1 : 0;
	}
	else
	{
		record >> ixfe;
	}
}
void Cell::save(CFRecord& record)
{
	auto globInfo = record.getGlobalWorkbookInfo();
	if(ixfe != 0)
	{
		ixfe+= globInfo->cellStyleXfs_count;
	}
    record << rw << col << ixfe;
}

const CellRef Cell::getLocation() const
{
	return RgceLoc(rw, col, true, true);
}


} // namespace XLS
