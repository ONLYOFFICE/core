#include "stdafx.h"
#include "URLMoniker.h"
#include <XLS_bin/CFRecord.h>

namespace OSHARED
{;


URLMoniker::URLMoniker()
{
}


URLMoniker::URLMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr URLMoniker::clone()
{
	return XLS::BiffStructurePtr(new URLMoniker(*this));
}


void URLMoniker::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"url", STR::escapeUrlW(url).c_str());
	if(!!serialGUID)
	{
		xml_tag->setAttribute(L"serialGUID", serialGUID);
		xml_tag->setAttribute(L"serialVersion", serialVersion);
		uriFlags.toXML(xml_tag);
	}
}


void URLMoniker::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	url = static_cast<wchar_t*>(static_cast<_bstr_t>(getStructAttribute(xml_tag, L"url")));
}


void URLMoniker::store(XLS::CFRecord& record)
{
	DWORD length = (url.length() + 1) * 2;
	record << length << url;
}


void URLMoniker::load(XLS::CFRecord& record)
{
	DWORD length;
	record >> length >> url;

	if(!record.isEOF())
	{
		GUID guid;
		record >> guid >> serialVersion >> uriFlags;
		serialGUID = STR::guid2bstr(guid);
	}
}


} // namespace OSHARED

