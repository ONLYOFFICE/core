#include "stdafx.h"
#include "PhRuns.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PhRuns::clone()
{
	return BiffStructurePtr(new PhRuns(*this));
}


void PhRuns::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"ichFirst", ichFirst);
	xml_tag->setAttribute(L"ichFirst", ichMom);
	xml_tag->setAttribute(L"ichFirst", cchMom);
}

void PhRuns::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	ichFirst = getStructAttribute(xml_tag, L"ichFirst");
	ichMom = getStructAttribute(xml_tag, L"ichFirst");
	cchMom = getStructAttribute(xml_tag, L"ichFirst");
}


void PhRuns::store(CFRecord& record)
{
	record << ichFirst << ichMom << cchMom;
}


void PhRuns::load(CFRecord& record)
{
	record >> ichFirst >> ichMom >> cchMom;
}


} // namespace XLS

