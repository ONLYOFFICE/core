#include "stdafx.h"
#include "PtgElfRadical.h"
#include "PtgParam.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgElfRadical::clone()
{
	return BiffStructurePtr(new PtgElfRadical(*this));
}


void PtgElfRadical::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	loc.toXML(xml_tag);
}


void PtgElfRadical::storeFields(CFRecord& record)
{
	record << loc;
}


void PtgElfRadical::loadFields(CFRecord& record)
{
	record >> loc;
}


void PtgElfRadical::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
	// The significant tag follows (PtgArea or PtgAreaErr)
	PtgParam param(PtgParam::ptELF_RADICAL, L"");
	param.addParam(loc.getColumnRelative());
	ptg_stack.push(param.toString());
}

} // namespace XLS

