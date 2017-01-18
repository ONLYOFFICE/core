/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "PtgArea3d.h"
#include "RevExtern.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

#include <boost/algorithm/string.hpp>

namespace XLS
{


PtgArea3d::PtgArea3d(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgArea3d::PtgArea3d(const unsigned short ixti_init, const std::wstring& ref_str, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	area(boost::algorithm::to_upper_copy(ref_str)),
	ixti(ixti_init),
	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr PtgArea3d::clone()
{
	return BiffStructurePtr(new PtgArea3d(*this));
}

void PtgArea3d::loadFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	
	if (global_info->Version < 0x0600)
	{
		unsigned short	rwFirst, rwLast;
		unsigned char	colFirst, colLast;

		record >> ixals;
		record .skipNunBytes(8);
		record >> itabFirst >> itabLast >> rwFirst >> rwLast >> colFirst >> colLast;

		area.rowFirstRelative	= rwFirst & 0x8000;
		area.columnLastRelative	= rwFirst & 0x4000;
		
		area.columnFirst		= colFirst;
		area.rowFirst			= rwFirst & 0x3FFF;

		area.rowLastRelative	= rwLast & 0x8000;
		area.columnLastRelative	= rwLast & 0x4000;
		
		area.columnLast			= colLast;
		area.rowLast			= rwLast & 0x3FFF;

	}
	else
	{
		record >> ixti;
		record >> area;
		area_rel = area;
	}
}


void PtgArea3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
#pragma message("####################### PtgArea3d struct for revisions is not implemented")
		Log::info("PtgArea3d struct for revisions is not implemented.");
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}
	std::wstring range_ref = area.toString();

	if (global_info->Version < 0x0600)
	{
		if (ixals == 0xffff)
		{
			std::wstring prefix = XMLSTUFF::xti_indexes2sheet_name(itabFirst, itabLast, global_info->sheets_names);
			if (!prefix.empty()) prefix += L"!";

			ptg_stack.push(prefix + range_ref);		
		}
		else
		{//external !!
			ptg_stack.push(XMLSTUFF::make3dRef(ixals, range_ref, global_info->xti_parsed, full_ref));
		}
	}
	else
	{
		ptg_stack.push(XMLSTUFF::make3dRef(ixti, range_ref, global_info->xti_parsed, full_ref));
	}

}


} // namespace XLS

