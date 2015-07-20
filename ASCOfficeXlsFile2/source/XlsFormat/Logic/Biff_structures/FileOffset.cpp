
#include "FileOffset.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FileOffset::clone()
{
	return BiffStructurePtr(new FileOffset(*this));
}


//void FileOffset::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"offset", offset);
//}
//
//
//void FileOffset::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	offset = getStructAttribute(xml_tag, L"offset");
//}


void FileOffset::store(CFRecord& record)
{
	record << offset;
}


void FileOffset::load(CFRecord& record)
{
	record >> offset;
}


} // namespace XLS

