#include "stdafx.h"
#include "ISSTInf.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ISSTInf::clone()
{
	return BiffStructurePtr(new ISSTInf(*this));
}


void ISSTInf::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"ib", ib);
	xml_tag->setAttribute(L"cbOffset", cbOffset);
}

void ISSTInf::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	ib = getStructAttribute(xml_tag, L"ib");
	cbOffset = getStructAttribute(xml_tag, L"cbOffset");
}


void ISSTInf::store(CFRecord& record)
{
	record << ib << cbOffset;
	record.reserveNBytes(2); // reserved
}


void ISSTInf::load(CFRecord& record)
{
	record >> ib >> cbOffset;
	record.skipNBytes(2); // reserved
}


} // namespace XLS

