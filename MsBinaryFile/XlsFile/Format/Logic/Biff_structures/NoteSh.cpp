/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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

