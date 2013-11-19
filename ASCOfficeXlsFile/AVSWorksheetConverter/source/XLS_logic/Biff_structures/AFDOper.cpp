#include "stdafx.h"
#include "AFDOper.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr AFDOper::clone()
{
	return BiffStructurePtr(new AFDOper(*this));
}

AFDOper::AFDOper()
{	
	m_bAutoFilter = true;
}

AFDOper::AFDOper(bool bAutoFilter)
{
	m_bAutoFilter = bAutoFilter;
}


AFDOper::~AFDOper()
{
}


void AFDOper::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
	xml_tag->setAttribute(L"vt", vt);
	xml_tag->setAttribute(L"grbitSign", grbitSign);
	vtValue.toXML(xml_tag);
}


void AFDOper::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
}


void AFDOper::store(CFRecord& record)
{	
}


void AFDOper::load(CFRecord& record)
{
	record >> vt >> grbitSign;

	BYTE _vt = vt;
	BYTE _grbitSign = grbitSign;

	if (vt == 0)
		record.skipNBytes(8);
	if (vt == 0x06)
	{
		vtValue.m_bAutoFilter = m_bAutoFilter;
		record >> vtValue;
	}
}


} // namespace XLS

