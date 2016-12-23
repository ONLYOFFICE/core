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

#include "PtgRef3d.h"
#include "RevExtern.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

#include <boost/algorithm/string.hpp>

namespace XLS
{


PtgRef3d::PtgRef3d(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgRef3d::PtgRef3d(const unsigned short ixti_init, const std::wstring& ref_str, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	rgce_loc(boost::algorithm::to_upper_copy(ref_str)),
	ixti(ixti_init),
	cell_base_ref(cell_base_ref_init)
{
}


BiffStructurePtr PtgRef3d::clone()
{
	return BiffStructurePtr(new PtgRef3d(*this));
}


void PtgRef3d::loadFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	
	if (global_info->Version < 0x0600)
	{
		_UINT16			rw;
		unsigned char	col;		
		record >> ixals;
		record.skipNunBytes(8);

		record >> itabFirst >> itabLast >> rw >> col;

		rgce_loc.rowRelative	= GETBIT(rw, 15);
		rgce_loc.colRelative	= GETBIT(rw, 14);

		rgce_loc.column			= col;
		rgce_loc.row			= GETBITS(rw, 0, 13);
	}
	else
	{
		record >> ixti >> rgce_loc;
	
		rgce_loc_rel = rgce_loc;
	}
	
}


void PtgRef3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
#pragma message("####################### PtgRef3d struct for revisions is not implemented")
		Log::info("PtgRef3d struct for revisions is not implemented.");
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}
	std::wstring cell_ref = rgce_loc.toString();

	if (global_info->Version < 0x0600)
	{
		if (ixals == 0xffff)
		{
			std::wstring prefix = XMLSTUFF::xti_indexes2sheet_name(itabFirst, itabLast, global_info->sheets_names);
			if (!prefix.empty()) prefix += L"!";

			ptg_stack.push(prefix + cell_ref);
		}
		else
		{
			ptg_stack.push(XMLSTUFF::make3dRef(ixals, cell_ref, global_info->xti_parsed, full_ref)); // from External !
		}
	}
	else
	{
		cell_ref = XMLSTUFF::make3dRef(ixti, cell_ref, global_info->xti_parsed, full_ref);
		ptg_stack.push(cell_ref);
	}
}


} // namespace XLS

