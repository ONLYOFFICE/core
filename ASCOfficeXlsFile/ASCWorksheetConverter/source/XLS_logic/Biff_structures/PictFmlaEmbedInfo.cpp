#include "stdafx.h"
#include "PictFmlaEmbedInfo.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PictFmlaEmbedInfo::clone()
{
	return BiffStructurePtr(new PictFmlaEmbedInfo(*this));
}


void PictFmlaEmbedInfo::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"strClass", strClass);
}


void PictFmlaEmbedInfo::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	strClass = static_cast<_bstr_t>(getStructAttribute(own_tag, L"strClass"));
}


void PictFmlaEmbedInfo::store(CFRecord& record)
{
	BYTE ttb = 0x03; // reserved
	record << ttb;

	WORD cbClass = static_cast<BYTE>(strClass.getSize());
	record << cbClass;
	record.reserveNBytes(1); // reserved

	record << strClass;
}


void PictFmlaEmbedInfo::load(CFRecord& record)
{
	record.skipNBytes(1); // reserved

	BYTE cbClass;
	record >> cbClass;
	record.skipNBytes(1); // reserved

	strClass.setSize(cbClass);
	record >> strClass;
}


} // namespace XLS

