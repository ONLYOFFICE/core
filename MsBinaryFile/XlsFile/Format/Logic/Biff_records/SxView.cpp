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

#include "SxView.h"

namespace XLS
{
	
BaseObjectPtr SxView::clone()
{
	return BaseObjectPtr(new SxView(*this));
}

void SxView::readFields(CFRecord& record)
{
	short reserved, flags;
	record >> ref >> rwFirstHead >> rwFirstData >> colFirstData >> iCache >> reserved;
	record >> sxaxis4Data >> ipos4Data;
	record >> cDim >> cDimRw >> cDimCol >> cDimPg >> cDimData >> cRw >> cCol;

	record >> flags;
	fRwGrand	= GETBIT(flags, 0);
	fColGrand	= GETBIT(flags, 1);
	fAutoFormat	= GETBIT(flags, 3);
	fAtrNum		= GETBIT(flags, 4);
	fAtrFnt		= GETBIT(flags, 5);
	fAtrAlc		= GETBIT(flags, 6);
	fAtrBdr		= GETBIT(flags, 7);
	fAtrPat		= GETBIT(flags, 8);
	fAtrProc	= GETBIT(flags, 9);

	record >> itblAutoFmt >> cchTableName >> cchDataName;

	if(cchTableName && cchTableName <= 0x00FF)
	{
		stTable.setSize(cchTableName);
		record >> stTable;
	}
	if(cchDataName > 0 && cchDataName <= 0x00FE)
	{
		stData.setSize(cchDataName);
		record >> stData;
	}
	int skip = record.getDataSize() - record.getRdPtr();
	record.skipNunBytes(skip);
}

void SxView::writeFields(CFRecord& record)
{
	auto globInfo = record.getGlobalWorkbookInfo();
	auto cacheIndex = globInfo->mapPivotCacheIndex.find(iCache);
	if(cacheIndex != globInfo->mapPivotCacheIndex.end())
		iCache = cacheIndex->second;
    unsigned short flags = 0;
    record << ref << rwFirstHead << rwFirstData << colFirstData << iCache;
    record.reserveNunBytes(2);
    record << sxaxis4Data << ipos4Data;
    record << cDim << cDimRw << cDimCol << cDimPg << cDimData << cRw << cCol;

    SETBIT(flags, 0, fRwGrand)
    SETBIT(flags, 1, fColGrand)
    SETBIT(flags, 3, fAutoFormat)
    SETBIT(flags, 4, fAtrNum)
    SETBIT(flags, 5, fAtrFnt)
    SETBIT(flags, 6, fAtrAlc)
    SETBIT(flags, 7, fAtrBdr)
    SETBIT(flags, 8, fAtrPat)
    SETBIT(flags, 9, fAtrProc)
    record << flags;

    cchTableName = stTable.getSize();
    cchDataName = stData.getSize();
    record << itblAutoFmt << cchTableName << cchDataName;
    if(cchTableName && cchTableName <= 0x00FF)
    {
        record << stTable;
    }
    if(cchDataName > 0 && cchDataName <= 0x00FE)
    {
        record << stData;
    }

}

} // namespace XLS

