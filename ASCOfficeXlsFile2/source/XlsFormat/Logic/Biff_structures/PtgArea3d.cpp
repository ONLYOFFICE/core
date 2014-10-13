#include "precompiled_xls.h"
#include "PtgArea3d.h"
#include "RevExtern.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgArea3d::PtgArea3d(const CellRef& cell_base_ref_init)
:	cell_base_ref(cell_base_ref_init)
{
}


PtgArea3d::PtgArea3d(const unsigned __int16 ixti_init, const std::wstring& ref_str, const PtgDataType data_type, const CellRef& cell_base_ref_init)
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


//void PtgArea3d::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ixti", ixti);
//
//	area.toXML(xml_tag);
//}


void PtgArea3d::storeFields(CFRecord& record)
{
	record << ixti;
	record << area; // Store absolutely always since no specific cell_base_ref specified in NameParsedFormula
}


void PtgArea3d::loadFields(CFRecord& record)
{
	record >> ixti;
	record >> area;
	area_rel = area;
	global_info = record.getGlobalWorkbookInfo();
}


void PtgArea3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
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

	ptg_stack.push(XMLSTUFF::make3dRef(ixti, range_ref, global_info->xti_parsed));

}


} // namespace XLS

