
#include "PtgErr.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


PtgErr::PtgErr()
{
}


PtgErr::PtgErr(const std::wstring  str)
:	err(str),
	OperandPtg(fixed_id)
{
}


BiffStructurePtr PtgErr::clone()
{
	return BiffStructurePtr(new PtgErr(*this));
}

//
//void PtgErr::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"err", err);
//}
//

void PtgErr::storeFields(CFRecord& record)
{
	record << err;
}


void PtgErr::loadFields(CFRecord& record)
{
	record >> err;
}


void PtgErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data)
{
	ptg_stack.push(err.toString());
}



} // namespace XLS

