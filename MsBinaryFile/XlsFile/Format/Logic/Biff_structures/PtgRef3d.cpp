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

#include "PtgRef3d.h"
#include "RevExtern.h"
#include "CellRef.h"

#include <boost/algorithm/string.hpp>

namespace XLS
{


PtgRef3d::PtgRef3d(const unsigned short full_ptg_id, const CellRef& cell_base_ref_init) : OperandPtg(full_ptg_id), cell_base_ref(cell_base_ref_init)
{
}

PtgRef3d::PtgRef3d(const unsigned short ixti_init, const std::wstring& ref_str, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	rgce_loc(boost::algorithm::to_upper_copy(ref_str)),
    rgce_loc_xlsb(boost::algorithm::to_upper_copy(ref_str)),
	ixti(ixti_init),
	cell_base_ref(cell_base_ref_init)
{
}

void PtgRef3d::set_base_ref(const CellRef& cell_base_ref_new)
{
	cell_base_ref = cell_base_ref_new;
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
    else if (global_info->Version < 0x0800)
    {
        record >> ixti >> rgce_loc;

        rgce_loc_rel = rgce_loc;
    }
    else
    {
        record >> ixti >> rgce_loc_xlsb;
    }
	
}


void PtgRef3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
		Log::info("PtgRef3d struct for revisions is not assemble.");
		
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}
    std::wstring cell_ref;
    if (global_info->Version < 0x0800)
    {
       cell_ref = rgce_loc.toString();
    }
    else
    {
        cell_ref = rgce_loc_xlsb.toString();
    }

	if (global_info->Version < 0x0600)
	{
		ixti = ixals;
		if (ixals == 0xffff)
		{
			std::wstring strRange;
			if(-1 == itabFirst)
			{
				strRange = L"#REF";
			}
			else
			{
				strRange = XMLSTUFF::name2sheet_name(global_info->sheets_info[itabFirst].name, L"");
				if (itabFirst != itabLast)
				{
					strRange += std::wstring(L":") + XMLSTUFF::name2sheet_name(global_info->sheets_info[itabLast].name, L"");
				}
			}
			if (!strRange.empty()) strRange += L"!";

			ptg_stack.push(strRange + cell_ref);
		}
	}
	if (ixti != 0xffff)
	{
		std::wstring link = ixti < global_info->arXti_External.size() ? global_info->arXti_External[ixti].link : L"";
		if (!link.empty() && !cell_ref.empty()) 
			link += L"!";
		
		if (full_ref && link.empty()) //Stock symbols comparison1.xls defined name "check_phrase"
		{
			link = L"#REF!";
		}
		ptg_stack.push(link + cell_ref);
	}
}


} // namespace XLS

