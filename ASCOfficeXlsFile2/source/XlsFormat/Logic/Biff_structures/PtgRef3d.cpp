#include "precompiled_xls.h"
#include "PtgRef3d.h"
#include "RevExtern.h"
#include "CellRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


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

//
//void PtgRef3d::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	static std::wstring  ixti_name(L"ixti");
//	xml_tag->setAttribute(ixti_name, ixti);
//
//	rgce_loc.toXML(xml_tag);
//}


void PtgRef3d::storeFields(CFRecord& record)
{
	rgce_loc_rel = rgce_loc;
	record << ixti << rgce_loc;
}


void PtgRef3d::loadFields(CFRecord& record)
{
	record >> ixti >> rgce_loc;
	rgce_loc_rel = rgce_loc;
	global_info = record.getGlobalWorkbookInfo();
}


void PtgRef3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
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

	ptg_stack.push(XMLSTUFF::make3dRef(ixti, cell_ref, global_info->xti_parsed));
}


} // namespace XLS

