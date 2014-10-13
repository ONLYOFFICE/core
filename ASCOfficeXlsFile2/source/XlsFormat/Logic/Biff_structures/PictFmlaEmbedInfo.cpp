#include "precompiled_xls.h"
#include "PictFmlaEmbedInfo.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr PictFmlaEmbedInfo::clone()
{
	return BiffStructurePtr(new PictFmlaEmbedInfo(*this));
}

//
//void PictFmlaEmbedInfo::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	own_tag->setAttribute(L"strClass", strClass);
//}
//
//
//void PictFmlaEmbedInfo::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	strClass = static_cast<std::wstring >(getStructAttribute(own_tag, L"strClass"));
//}


void PictFmlaEmbedInfo::store(CFRecord& record)
{
	unsigned char ttb = 0x03; // reserved
	record << ttb;

	unsigned __int16 cbClass = static_cast<unsigned char>(strClass.getSize());
	record << cbClass;
	record.reserveNunBytes(1); // reserved

	record << strClass;
}


void PictFmlaEmbedInfo::load(CFRecord& record)
{
	record.skipNunBytes(1); // reserved

	unsigned char cbClass;
	record >> cbClass;
	record.skipNunBytes(1); // reserved

	strClass.setSize(cbClass);
	record >> strClass;
}


} // namespace XLS

