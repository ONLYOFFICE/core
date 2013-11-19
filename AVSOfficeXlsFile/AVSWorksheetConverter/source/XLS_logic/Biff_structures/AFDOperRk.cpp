#include "stdafx.h"
#include "AFDOperRk.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr AFDOperRk::clone()
{
	return BiffStructurePtr(new AFDOperRk(*this));
}


void AFDOperRk::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
	xml_tag->setAttribute(L"rk", rk);	
}


void AFDOperRk::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
}


void AFDOperRk::store(CFRecord& record)
{	
}


void AFDOperRk::load(CFRecord& record)
{
	// rk (4 bytes): An RkNumber that specifies a numeric value.
	// unused1 (4 bytes): Undefined and MUST be ignored.

	record >> rk;
	record.skipNBytes(4);

	DWORD _rk = rk;
}


} // namespace XLS

