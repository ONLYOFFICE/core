
#include "PtgMemFunc.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr PtgMemFunc::clone()
{
	return BiffStructurePtr(new PtgMemFunc(*this));
}


//void PtgMemFunc::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"cce", cce);
//}


void PtgMemFunc::storeFields(CFRecord& record)
{
	record << cce;
}


void PtgMemFunc::loadFields(CFRecord& record)
{
	record >> cce;
}


void PtgMemFunc::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	// no textual form
}


} // namespace XLS

