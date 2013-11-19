#include "stdafx.h"
#include "Phs.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr Phs::clone()
{
	return BiffStructurePtr(new Phs(*this));
}


void Phs::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"ifnt", ifnt);
	xml_tag->setAttribute(L"phType", data.phType);
	xml_tag->setAttribute(L"alcH", data.alcH);
}

void Phs::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	ifnt = getStructAttribute(xml_tag, L"ifnt");
	data.phType = getStructAttribute(xml_tag, L"phType");
	data.alcH = getStructAttribute(xml_tag, L"alcH");
	data.unused = 0;
	data.set_one = 0xff;
}


void Phs::store(CFRecord& record)
{
	record << ifnt;
	record.storeAnyData(data);
}


void Phs::load(CFRecord& record)
{
	record >> ifnt;
	record.loadAnyData(data);
}


} // namespace XLS
