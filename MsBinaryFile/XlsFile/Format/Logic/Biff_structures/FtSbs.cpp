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

#include "FtSbs.h"

namespace XLS
{

BiffStructurePtr FtSbs::clone()
{
	return BiffStructurePtr(new FtSbs(*this));
}

void FtSbs::load(CFRecord& record)
{
	unsigned short ft, cb;
	record >> ft >> cb;

	if ( ft != 0x000c && cb != 0x0014)
	{
		record.RollRdPtrBack(4);
		return;
	}
	fExist = true;

	record.skipNunBytes(4); // unused1

	unsigned short flags;

	record >> iVal >> iMin >> iMax >> dInc >> dPage >> fHoriz >> dxScroll >> flags;

	fDraw			= GETBIT(flags, 0);
	fDrawSliderOnly = GETBIT(flags, 1);
	fTrackElevator	= GETBIT(flags, 2);
	fNo3d			= GETBIT(flags, 3);
}

void FtSbs::save(CFRecord& record)
{
	{
		unsigned short ft = 0x000C, cb = 0x0014;
		record << ft << cb;
		record.reserveNunBytes(4);//reserved
	}
	unsigned short flags = 0;
	SETBIT(flags, 0, fDraw)
	SETBIT(flags, 1, fDrawSliderOnly)
	SETBIT(flags, 2, fTrackElevator)
	SETBIT(flags, 3, fNo3d)
	record << iVal << iMin << iMax << dInc << dPage << fHoriz << dxScroll << flags;
}


} // namespace XLS

