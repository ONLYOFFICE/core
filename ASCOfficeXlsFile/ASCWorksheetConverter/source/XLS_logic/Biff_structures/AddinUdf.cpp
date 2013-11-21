#include "stdafx.h"
#include "AddinUdf.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr AddinUdf::clone()
{
	return BiffStructurePtr(new AddinUdf(*this));
}


void AddinUdf::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"udfName", udfName);
}


void AddinUdf::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	udfName = static_cast<_bstr_t>(getStructAttribute(xml_tag, L"udfName"));
}


void AddinUdf::store(CFRecord& record)
{
	record.reserveNBytes(4); // reserved
	record << udfName;
	WORD cb = 0;
	record << cb;
	record.reserveNBytes(cb); // unused
}


void AddinUdf::load(CFRecord& record)
{
	record.skipNBytes(4); // reserved
	record >> udfName;
	record.getGlobalWorkbookInfo()->AddinUdfs.push_back(udfName);

	WORD cb;
	record >> cb;
	record.skipNBytes(cb); // unused
}


} // namespace XLS

