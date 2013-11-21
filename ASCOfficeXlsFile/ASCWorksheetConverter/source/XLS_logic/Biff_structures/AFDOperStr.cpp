#include "stdafx.h"
#include "AFDOperStr.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr AFDOperStr::clone()
{
	return BiffStructurePtr(new AFDOperStr(*this));
}

AFDOperStr::AFDOperStr()
{
	m_bAutoFilter = true;
}

AFDOperStr::AFDOperStr(bool bAutoFilter)
{
	m_bAutoFilter = bAutoFilter;
}


AFDOperStr::~AFDOperStr()
{
}


void AFDOperStr::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
	xml_tag->setAttribute(L"cch", cch);
	xml_tag->setAttribute(L"fCompare", fCompare);	
}


void AFDOperStr::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
}


void AFDOperStr::store(CFRecord& record)
{	
}


void AFDOperStr::load(CFRecord& record)
{	
	if (m_bAutoFilter)		// AutoFilter
		record.skipNBytes(4);

	record >> cch >> fCompare;

	BYTE _cch = cch;
	BYTE _fCompare = fCompare;

	record.skipNBytes(1);	// reserved1
	record.skipNBytes(1);	// unused2

	if (!m_bAutoFilter)		// AutoFilter12
		record.skipNBytes(4);
}


} // namespace XLS

