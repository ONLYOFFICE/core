
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

void PtgArea3d::storeFields(CFRecord& record)
{
	record << ixti;
	record << area; 
}


void PtgArea3d::loadFields(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	
	if (global_info->Version < 0x0600)
	{
		record >> ixals;
		record .skipNunBytes(8);
		record >> itabFirst >> itabLast >> grbitRwFirst >> grbitRwLast >> colFirst >> colLast;
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

	if (global_info->Version < 0x0600)
	{
		CellRef cell1(grbitRwFirst, colFirst,  false, false);
		CellRef cell2(grbitRwLast, colLast, false, false);

		CellRangeRef range(cell1, cell2);

		std::wstring range_ref = range.toString();
		
		if (ixals == 0xffff)//current sheet
			ptg_stack.push(XMLSTUFF::make3dRef(global_info->current_sheet - 1, range_ref, global_info->sheets_names, full_ref));
		else
			ptg_stack.push(XMLSTUFF::make3dRef(ixals, range_ref, global_info->xti_parsed, full_ref));
	}
	else
	{
		std::wstring range_ref = area.toString();
		ptg_stack.push(XMLSTUFF::make3dRef(ixti, range_ref, global_info->xti_parsed, full_ref));
	}

}


} // namespace XLS

