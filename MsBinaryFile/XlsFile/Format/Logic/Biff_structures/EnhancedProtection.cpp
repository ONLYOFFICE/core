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

#include "EnhancedProtection.h"

namespace XLS
{


EnhancedProtection::EnhancedProtection()
{
}


EnhancedProtection::EnhancedProtection(CFRecord& record)
{
	load(record);
}


BiffStructurePtr EnhancedProtection::clone()
{
	return BiffStructurePtr(new EnhancedProtection(*this));
}

void EnhancedProtection::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;

	iprotObjects = GETBIT(flags, 0);
	iprotScenarios = GETBIT(flags, 1);
	iprotFormatCells = GETBIT(flags, 2);
	iprotFormatColumns = GETBIT(flags, 3);
	iprotFormatRows = GETBIT(flags, 4);
	iprotInsertColumns = GETBIT(flags, 5);
	iprotInsertRows = GETBIT(flags, 6);
	iprotInsertHyperlinks = GETBIT(flags, 7);
	iprotDeleteColumns = GETBIT(flags, 8);
	iprotDeleteRows = GETBIT(flags, 9);
	iprotSelLockedCells = GETBIT(flags, 10);
	iprotSort = GETBIT(flags, 11);
	iprotAutoFilter = GETBIT(flags, 12);
	iprotPivotTables = GETBIT(flags, 13);
	iprotSelUnlockedCells = GETBIT(flags, 14);
	record.skipNunBytes(2); // reserved
}

void EnhancedProtection::save(CFRecord& record)
{
	unsigned short flags = 0;

	SETBIT(flags, 0, iprotObjects)
	SETBIT(flags, 1, iprotScenarios)
	SETBIT(flags, 2, iprotFormatCells)
	SETBIT(flags, 3, iprotFormatColumns)
	SETBIT(flags, 4, iprotFormatRows)
	SETBIT(flags, 5, iprotInsertColumns)
	SETBIT(flags, 6, iprotInsertRows)
	SETBIT(flags, 7, iprotInsertHyperlinks)
	SETBIT(flags, 8, iprotDeleteColumns)
	SETBIT(flags, 9, iprotDeleteRows)
	SETBIT(flags, 10, iprotSelLockedCells)
	SETBIT(flags, 11, iprotSort)
	SETBIT(flags, 12, iprotAutoFilter)
	SETBIT(flags, 13, iprotPivotTables)
	SETBIT(flags, 14, iprotSelUnlockedCells)

	record << flags;
	record.reserveNunBytes(2);
}

} // namespace XLS

