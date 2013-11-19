#include "stdafx.h"
#include "OfficeArtIDCL.h"
#include <XLS_bin/CFRecord.h>

namespace ODRAW
{;


XLS::BiffStructurePtr OfficeArtIDCL::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtIDCL(*this));
}


void OfficeArtIDCL::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"dgid", dgid);
	xml_tag->setAttribute(L"cspidCur", cspidCur);
}

void OfficeArtIDCL::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	dgid = getStructAttribute(xml_tag, L"dgid");
	cspidCur = getStructAttribute(xml_tag, L"cspidCur");
}


void OfficeArtIDCL::store(XLS::CFRecord& record)
{
	record << dgid << cspidCur;
}


void OfficeArtIDCL::load(XLS::CFRecord& record)
{
	record >> dgid >> cspidCur;
}


} // namespace XLS
