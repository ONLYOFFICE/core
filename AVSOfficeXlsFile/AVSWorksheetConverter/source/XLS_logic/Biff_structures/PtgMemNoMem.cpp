#include "stdafx.h"
#include "PtgMemNoMem.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PtgMemNoMem::clone()
{
	return BiffStructurePtr(new PtgMemNoMem(*this));
}


void PtgMemNoMem::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"cce", cce);
}


void PtgMemNoMem::storeFields(CFRecord& record)
{
	record.reserveNBytes(4); // unused
	record << cce;
}


void PtgMemNoMem::loadFields(CFRecord& record)
{
	record.skipNBytes(4); // unused
	record >> cce;
}


void PtgMemNoMem::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, MSXML2::IXMLDOMElementPtr parent)
{
// I assume it doesn't generate an error.
}



} // namespace XLS

