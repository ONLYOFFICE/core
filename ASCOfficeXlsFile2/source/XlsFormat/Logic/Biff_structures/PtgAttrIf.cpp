#include "precompiled_xls.h"
#include "PtgAttrIf.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgAttrIf::clone()
{
	return BiffStructurePtr(new PtgAttrIf(*this));
}

//
//void PtgAttrIf::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"offset", offset);
//}


void PtgAttrIf::storeFields(CFRecord& record)
{
	record << offset;
}


void PtgAttrIf::loadFields(CFRecord& record)
{
	record >> offset;
}


void PtgAttrIf::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
	// All control tokens shall be skipped
}


} // namespace XLS

