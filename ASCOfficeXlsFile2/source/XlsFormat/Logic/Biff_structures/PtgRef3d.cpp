
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

void PtgRef3d::storeFields(CFRecord& record)
{
	rgce_loc_rel = rgce_loc;
	record << ixti << rgce_loc;
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

