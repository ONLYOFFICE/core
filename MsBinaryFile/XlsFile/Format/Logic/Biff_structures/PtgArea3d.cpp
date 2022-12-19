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

#include "PtgArea3d.h"
#include "RevExtern.h"
#include "CellRangeRef.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

PtgArea3d::PtgArea3d(const unsigned short full_ptg_id, const CellRef& cell_base_ref_init) : OperandPtg(full_ptg_id), cell_base_ref(cell_base_ref_init)
{
}

PtgArea3d::PtgArea3d(const unsigned short ixti_init, const std::wstring& ref_str, const PtgDataType data_type, const CellRef& cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	area(ref_str),
    areaXlsb(ref_str),
	ixti(ixti_init),
	cell_base_ref(cell_base_ref_init)
{
	XmlUtils::GetUpper(area);
}

void PtgArea3d::set_base_ref(const CellRef& cell_base_ref_new)
{
	cell_base_ref = cell_base_ref_new;
}

BiffStructurePtr PtgArea3d::clone()
{
	return BiffStructurePtr(new PtgArea3d(*this));
}

void PtgArea3d::loadFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	
	if (global_info->Version < 0x0600)
	{//biff5/7
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
    else if (global_info->Version < 0x0800)
    {//biff8
        record >> ixti;
        record >> area;
        area_rel = area;
    }
    else
    {//biff12
        record >> ixti;
        record >> areaXlsb;
    }

}


void PtgArea3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
		Log::info("PtgArea3d struct for revisions is not assemble.");
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}
    std::wstring range_ref;
    if(global_info->Version < 0x0800)
        range_ref = area.toString();
    else
        range_ref = areaXlsb.toString();

	if (global_info->Version < 0x0600)
	{
		ixti = ixals;
		if (((short)ixals) < 0)
		{
			std::wstring strRange;
			if (-1 == itabFirst)
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

			ptg_stack.push(strRange + range_ref);		
		}
	}
	if (ixti < global_info->arXti_External.size())
	{
		std::wstring link = global_info->arXti_External[ixti].link;
		if (!link.empty() && !range_ref.empty()) 
			link += L"!";

		if (full_ref && link.empty()) //4673306.xls defined name "Категория"
		{
			link = L"#REF!";
		}
		
		ptg_stack.push(link + range_ref); 
	}

}


} // namespace XLS

