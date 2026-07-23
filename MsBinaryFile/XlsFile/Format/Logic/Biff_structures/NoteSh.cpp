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

#include "NoteSh.h"
#include "CellRef.h"

namespace XLS
{

NoteSh::NoteSh()
{
	x_ = y_ = 0;
	
	fShow		= false;
	fRwHidden	= false;
	fColHidden	= false;
    stAuthor = L" ";
}

NoteSh::NoteSh(CFRecord& record)
{
	x_ = y_ = 0;
	load(record);
}

BiffStructurePtr NoteSh::clone()
{
	return BiffStructurePtr(new NoteSh(*this));
}

void NoteSh::load(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();

	unsigned short flags;
	record >> row >> col;
	
	if (global_info->Version < 0x0600)
	{
		LPAnsiString stText1;
		record >> stText1;
		
		stText = stText1;
	}
	else
	{
		record >> flags;
		
		fShow		= GETBIT(flags, 1);
		fRwHidden	= GETBIT(flags, 7);
		fColHidden	= GETBIT(flags, 8);
		
		record >> idObj >> stAuthor;
	
		record.skipNunBytes(1); // unused
	}
}

void NoteSh::save(CFRecord& record)
{
    record << row << col;

    unsigned short flags = 0;
    SETBIT(flags, 0, fShow)
    SETBIT(flags, 7, fRwHidden)
    SETBIT(flags, 8, fColHidden)
    record << flags;
    record << idObj << stAuthor;
    record.reserveNunBytes(1);
}

//-----------------------------------------------------------------------
void NoteSh::calculate()
{
	XLS::GlobalWorkbookInfo::_sheet_info zero;
	XLS::GlobalWorkbookInfo::_sheet_info & sheet_info = global_info->current_sheet >= 0 ? 
										global_info->sheets_info[global_info->current_sheet - 1] : zero;

	ref_ = CellRef(row, col, true, true).toString();

	double kfCol	= 17640 / 256.;
	double kfRow	= ( 360000 * 2.54 / 72) / 256. ;

	for (int i = 0 ; i < col; i++)
	{
		if (sheet_info.customColumnsWidth.find(i) != sheet_info.customColumnsWidth.end())
			x_ +=  256 * kfCol * sheet_info.customColumnsWidth[i];	
		else 
			x_ +=  256 * kfCol * sheet_info.defaultColumnWidth;
	}

	for (int i = 0 ; i < row; i++)
	{
		if (sheet_info.customRowsHeight.find(i) != sheet_info.customRowsHeight.end())
		{
			y_ += 256 * kfRow * sheet_info.customRowsHeight[i];	
		}
		else 
			y_ += 256 * kfRow * sheet_info.defaultRowHeight;	
	}
}


} // namespace XLS

