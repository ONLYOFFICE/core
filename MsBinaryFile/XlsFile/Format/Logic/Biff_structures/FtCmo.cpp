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

#include "FtCmo.h"

namespace XLS
{


BiffStructurePtr FtCmo::clone()
{
	return BiffStructurePtr(new FtCmo(*this));
}
void FtCmo::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	record >> ot >> id;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		unsigned char flags;
		record >> flags;
		
		fLocked			= GETBIT(flags, 0);
		fDefaultSize	= GETBIT(flags, 2);
		fPublished		= GETBIT(flags, 3);
		fPrint			= GETBIT(flags, 4);
		fDisabled		= GETBIT(flags, 7);
		
		fUIObj			= false;
		fRecalcObj		= false;
		fRecalcObjAlways = false;
	}
	else
	{
		unsigned short flags;
		record >> flags;
		record.skipNunBytes(12); // unused

		fLocked			= GETBIT(flags, 0);
		fDefaultSize	= GETBIT(flags, 2);
		fPublished		= GETBIT(flags, 3);
		fPrint			= GETBIT(flags, 4);
		fDisabled		= GETBIT(flags, 7);
		fUIObj			= GETBIT(flags, 8);
		fRecalcObj		= GETBIT(flags, 9);
		fRecalcObjAlways = GETBIT(flags, 12);
	}
}

void FtCmo::save(CFRecord& record)
{
	{
		unsigned short ft = 0x15;
		record << ft;
		ft = 0x12;
		record << ft;// reserved
	}
	record << ot << id;
	unsigned short flags = 0;
	SETBIT(flags, 0, fLocked)
	SETBIT(flags, 2, fDefaultSize)
	SETBIT(flags, 3, fPublished)
	SETBIT(flags, 4, fPrint)
	SETBIT(flags, 7, fDisabled)
	SETBIT(flags, 8, fUIObj)
	SETBIT(flags, 9, fRecalcObj)
	SETBIT(flags, 12, fRecalcObjAlways)
	record << flags;
	record.reserveNunBytes(12);
}


} // namespace XLS

