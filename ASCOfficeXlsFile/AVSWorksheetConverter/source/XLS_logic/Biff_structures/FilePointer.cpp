#include "stdafx.h"
#include "FilePointer.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;

FilePointer::FilePointer()
:	offset(-1)
{
}


FilePointer::FilePointer(const DWORD ptr)
:	offset(ptr)
{
}


BiffStructurePtr FilePointer::clone()
{
	return BiffStructurePtr(new FilePointer(*this));
}


void FilePointer::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"offset", offset);
}


void FilePointer::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	offset = getStructAttribute(xml_tag, L"offset");
}


void FilePointer::store(CFRecord& record)
{
	record << offset;
}


void FilePointer::load(CFRecord& record)
{
	record >> offset;
}


} // namespace XLS

