#include "stdafx.h"
#include "PtgAreaErr3d.h"
#include "RevExtern.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAreaErr3d::clone()
{
	return BiffStructurePtr(new PtgAreaErr3d(*this));
}


void PtgAreaErr3d::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"ixti", ixti);
}


void PtgAreaErr3d::storeFields(CFRecord& record)
{
	record << ixti;
	record.reserveNBytes(8); // unused
}


void PtgAreaErr3d::loadFields(CFRecord& record)
{
	record >> ixti;
	record.skipNBytes(8); // unused
	global_info = record.getGlobalWorkbookInfo();
}


void PtgAreaErr3d::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	RevExternPtr tab_ids;
	if(!extra_data.empty() && (tab_ids = boost::dynamic_pointer_cast<RevExtern>(extra_data.front())))
	{
#pragma message("####################### PtgAreaErr3d struct for revisions is not implemented")
		Log::info("PtgAreaErr3d struct for revisions is not implemented.");
		ptg_stack.push(L"");
		extra_data.pop();
		return;
	}

	ptg_stack.push(XMLSTUFF::make3dRef(ixti, L"#REF!", global_info->xti_parsed));

}


} // namespace XLS

