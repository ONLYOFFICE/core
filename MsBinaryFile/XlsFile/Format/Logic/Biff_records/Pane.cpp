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

#include "Pane.h"

namespace XLS
{

Pane::Pane()
{
}


Pane::~Pane()
{
}


BaseObjectPtr Pane::clone()
{
	return BaseObjectPtr(new Pane(*this));
}


void Pane::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16		rwTop_2b;
        _UINT16		colLeft_2b;

        record >> x >> y >> rwTop_2b >> colLeft_2b >> pnnAcct;

        rwTop   = rwTop_2b;
        colLeft = colLeft_2b;

        topLeftCell = static_cast<std::wstring >(CellRef(rwTop_2b, colLeft_2b, true, true));
        record.skipNunBytes(1); // reserved
    }
    else
    {
        record >> xnumXSplit >> xnumYSplit >> rwTop >> colLeft >> pnnAcct_xlsb;
        topLeftCell = CellRef(rwTop, colLeft, true, true).toString(true);

        unsigned char flags;
        record >> flags;

        fFrozen             = GETBIT(flags, 0);
        fFrozenNoSplit      = GETBIT(flags, 1);

    }

}

void Pane::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		CellRef ref(topLeftCell);

		_UINT16		rwTop_2b = ref.row;
		_UINT16		colLeft_2b = ref.column;

		record << x << y << rwTop_2b << colLeft_2b << pnnAcct;
		
		record.reserveNunBytes(1); // reserved
	}
	else
	{
		CellRef ref(topLeftCell);

		rwTop = ref.row;
		colLeft = ref.column;

		record << xnumXSplit << xnumYSplit << rwTop << colLeft << pnnAcct_xlsb;

		BYTE flags = 0;

		SETBIT(flags, 0, fFrozen);
		SETBIT(flags, 1, fFrozenNoSplit);

		record << flags;
	}

}

} // namespace XLS

