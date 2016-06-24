/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "SortCond12.h"
#include <Binary/CFRecord.h>
#include "CellRangeRef.h"

namespace XLS
{


BiffStructurePtr SortCond12::clone()
{
	return BiffStructurePtr(new SortCond12(*this));
}

void SortCond12::store(CFRecord& record)
{
#pragma message("####################### SortCond12 record has no BiffStructure::store() implemented")
	Log::error(" Error!!! SortCond12 record has no BiffStructure::store() implemented.");
	//record << something;
}


void SortCond12::load(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fSortDes	= GETBIT(flags, 0);
	sortOn		= GETBITS(flags, 1, 4);

	record >> rfx;

	switch(sortOn)
	{
		case 0x00:
		case 0x01:
		case 0x02:
			record >> condDataValue;
			break;
		case 0x03:
			record >> cfflag;
			break;
	}

	record >> cchSt;
	if(cchSt)
	{
		stSslist.setSize(cchSt);
		record >> stSslist;
	}
}


} // namespace XLS

