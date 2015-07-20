
#include "PtgSxName.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgSxName::clone()
{
	return BiffStructurePtr(new PtgSxName(*this));
}


//void PtgSxName::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"sxIndex", sxIndex);
//}


void PtgSxName::storeFields(CFRecord& record)
{
	record << sxIndex;
}


void PtgSxName::loadFields(CFRecord& record)
{
	record >> sxIndex;
}


void PtgSxName::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, BiffStructurePtr & parent)
{
#pragma message("####################### PtgSxName struct is not implemented")
	Log::info("PtgSxName structure is not implemented.");
	ptg_stack.push(L"#REF!");
}


} // namespace XLS

