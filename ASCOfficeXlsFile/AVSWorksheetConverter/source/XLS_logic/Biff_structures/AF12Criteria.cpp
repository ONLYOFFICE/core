#include "stdafx.h"
#include "AF12Criteria.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr AF12Criteria::clone()
{
	return BiffStructurePtr(new AF12Criteria(*this));
}

AF12Criteria::AF12Criteria()
{		
}


AF12Criteria::~AF12Criteria()
{
}


void AF12Criteria::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
	xml_tag->setAttribute(L"str", _str);
}


void AF12Criteria::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
}


void AF12Criteria::store(CFRecord& record)
{	
}


void AF12Criteria::load(CFRecord& record)
{	
	doper.m_bAutoFilter = false;
	record >> doper;

	if (doper.vt == 0x06)
	{
		BYTE _cch = doper.vtValue.cch;

		str.setSize(doper.vtValue.cch);
		record >> str;
		_str = str.getEscaped_ST_Xstring();
	}
}


} // namespace XLS

