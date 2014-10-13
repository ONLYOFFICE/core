#include "precompiled_xls.h"
#include "PtgRefErr3d.h"
#include "RevExtern.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgRefErr3d::PtgRefErr3d()
{
}


PtgRefErr3d::PtgRefErr3d(const unsigned __int16 ixti_init, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	ixti(ixti_init)
{
}


BiffStructurePtr PtgRefErr3d::clone()
{
	return BiffStructurePtr(new PtgRefErr3d(*this));
}

//
//void PtgRefErr3d::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ixti", ixti);
//}


void PtgRefErr3d::storeFields(CFRecord& record)
{
	record << ixti;
	record.reserveNunBytes(4); // unused
}


void PtgRefErr3d::loadFields(CFRecord& record)
{
	record >> ixti;
	record.skipNunBytes(4); // unused
	global_info = record.getGlobalWorkbookInfo();
}


void PtgRefErr3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
#pragma message("####################### PtgRefErr3d struct for revisions is not implemented")
		Log::info("PtgRefErr3d struct for revisions is not implemented.");
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}

	ptg_stack.push(XMLSTUFF::make3dRef(ixti, L"#REF!", global_info->xti_parsed));
}


} // namespace XLS

