
#include "PtgAttrGoto.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgAttrGoto::clone()
{
	return BiffStructurePtr(new PtgAttrGoto(*this));
}


//void PtgAttrGoto::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"offset", offset);
//}


void PtgAttrGoto::storeFields(CFRecord& record)
{
	record << offset;
}


void PtgAttrGoto::loadFields(CFRecord& record)
{
	record >> offset;
}


void PtgAttrGoto::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	// All control tokens shall be skipped
}


} // namespace XLS

