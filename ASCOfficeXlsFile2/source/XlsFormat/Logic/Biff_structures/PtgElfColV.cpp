
#include "PtgElfColV.h"
#include "CellRangeRef.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgElfColV::clone()
{
	return BiffStructurePtr(new PtgElfColV(*this));
}


//void PtgElfColV::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	loc.toXML(xml_tag);
//}


void PtgElfColV::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgElfColV::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgElfColV::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(CellRangeRef(loc, CellRangeRef::stColumnBelow).toString());
}



} // namespace XLS

