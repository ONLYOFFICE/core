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
#include "ConnGrbitDbtOledb.h"

namespace XLS
{

BiffStructurePtr ConnGrbitDbtOledb::clone()
{
	return BiffStructurePtr(new ConnGrbitDbtOledb(*this));
}

void ConnGrbitDbtOledb::load(CFRecord& record)
{
	unsigned short flags;
	record 	>> flags;

	dbost					= GETBITS(flags, 0, 2);
	fLocalConn				= GETBIT(flags, 3);
	fNoRefreshCube			= GETBIT(flags, 4);
	fUseOfficeLcid			= GETBIT(flags, 5);
	fSrvFmtNum				= GETBIT(flags, 6);
	fSrvFmtBack				= GETBIT(flags, 7);
	fSrvFmtFore				= GETBIT(flags, 8);
	fSrvFmtFlags			= GETBIT(flags, 9);
	fSupportsLangCellProp	= GETBIT(flags, 10);
	fSrvSupportsClientCube	= GETBIT(flags, 11);
}

void ConnGrbitDbtOledb::save(CFRecord& record)
{
	unsigned short flags = 0;

	SETBITS(flags, 0, 2, dbost)
	SETBIT(flags, 3, fLocalConn)
	SETBIT(flags, 4, fNoRefreshCube)
	SETBIT(flags, 5, fUseOfficeLcid)
	SETBIT(flags, 6, fSrvFmtNum)
	SETBIT(flags, 7, fSrvFmtBack)
	SETBIT(flags, 8, fSrvFmtFore)
	SETBIT(flags, 9, fSrvFmtFlags)
	SETBIT(flags, 10, fSupportsLangCellProp)
	SETBIT(flags, 11, fSrvSupportsClientCube)

	record 	<< flags;
}

} // namespace XLS

