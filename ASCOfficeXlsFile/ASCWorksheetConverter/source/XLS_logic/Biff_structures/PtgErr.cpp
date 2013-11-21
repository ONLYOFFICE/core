#include "stdafx.h"
#include "PtgErr.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


PtgErr::PtgErr()
{
}


PtgErr::PtgErr(const _bstr_t str)
:	err(str),
	OperandPtg(fixed_id)
{
}


BiffStructurePtr PtgErr::clone()
{
	return BiffStructurePtr(new PtgErr(*this));
}


void PtgErr::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"err", err);
}


void PtgErr::storeFields(CFRecord& record)
{
	record << err;
}


void PtgErr::loadFields(CFRecord& record)
{
	record >> err;
}


void PtgErr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	ptg_stack.push(err.toString());
}



} // namespace XLS

