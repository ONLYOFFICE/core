#include "precompiled_xls.h"
#include "FtEdoData.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FtEdoData::clone()
{
	return BiffStructurePtr(new FtEdoData(*this));
}


//void FtEdoData::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	own_tag->setAttribute(L"ivtEdit", ivtEdit);
//	own_tag->setAttribute(L"fMultiLine", fMultiLine);
//	own_tag->setAttribute(L"fVScroll", fVScroll);
//	own_tag->setAttribute(L"id", id);
//}
//
//
//void FtEdoData::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	ivtEdit = getStructAttribute(own_tag, L"ivtEdit");
//	fMultiLine = static_cast<bool>(getStructAttribute(own_tag, L"fMultiLine"));
//	fVScroll = getStructAttribute(own_tag, L"fVScroll");
//	id = getStructAttribute(own_tag, L"id");
//}


void FtEdoData::store(CFRecord& record)
{
	unsigned short ft = 0x0010; // reserved
	unsigned short cb = 0x0008; // reserved
	record << ft << cb;

	record << ivtEdit << fMultiLine << fVScroll << id;
}


void FtEdoData::load(CFRecord& record)
{
	record.skipNunBytes(4); // reserved

	record >> ivtEdit >> fMultiLine >> fVScroll >> id;
}


} // namespace XLS

