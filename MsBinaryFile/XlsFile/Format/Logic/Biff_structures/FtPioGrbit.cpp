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

#include "FtPioGrbit.h"

namespace XLS
{


BiffStructurePtr FtPioGrbit::clone()
{
	return BiffStructurePtr(new FtPioGrbit(*this));
}

void FtPioGrbit::load(CFRecord& record)
{
	unsigned short ft, cb;
	record >> ft >> cb;

	if ( ft != 0x0008 || cb != 2)
	{
		record.RollRdPtrBack(4);
		return;
	}
	fExist = true;

	unsigned short flags;
	record >> flags;

	fAutoPict	= GETBIT(flags, 0);
	fDde		= GETBIT(flags, 1);
	fPrintCalc	= GETBIT(flags, 2);
	fIcon		= GETBIT(flags, 3);
	fCtl		= GETBIT(flags, 4);
	fPrstm		= GETBIT(flags, 5);
	fCamera		= GETBIT(flags, 7);
	fDefaultSize = GETBIT(flags, 8);
	fAutoLoad	= GETBIT(flags, 9);
}

void FtPioGrbit::save(CFRecord& record)
{
	unsigned short ft =  0x0008, cb = 0x0002;
	record << ft << cb;
	unsigned short flags = 0;

	SETBIT(flags, 0, fAutoPict)
	SETBIT(flags, 1, fDde)
	SETBIT(flags, 2, fPrintCalc)
	SETBIT(flags, 3, fIcon)
	SETBIT(flags, 4, fCtl)
	SETBIT(flags, 5, fPrstm)
	SETBIT(flags, 7, fCamera)
	SETBIT(flags, 8, fDefaultSize)
	SETBIT(flags, 9, fAutoLoad)

	record << flags;
}

} // namespace XLS

