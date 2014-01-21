#include "stdafx.h"
#include "FtNts.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/AttributeDataWrong.h>

namespace XLS
{;


BiffStructurePtr FtNts::clone()
{
	return BiffStructurePtr(new FtNts(*this));
}


void FtNts::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	own_tag->setAttribute(L"guid", guid);
	own_tag->setAttribute(L"fSharedNote", fSharedNote);
}


void FtNts::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
{
	guid = getStructAttribute(own_tag, L"guid");
	fSharedNote = static_cast<bool>(getStructAttribute(own_tag, L"fSharedNote"));
}


void FtNts::store(CFRecord& record)
{
	WORD ft = 0x0D; // reserved
	WORD cb = 0x16; // reserved
	record << ft << cb;

	GUID guid_num;
	if(!STR::bstr2guid(guid, guid_num))
	{
		throw EXCEPT::LE::AttributeDataWrong(L"guid", L"FtNts", guid);
	}
	record << guid_num << fSharedNote;
}


void FtNts::load(CFRecord& record)
{
	record.skipNBytes(4); // reserved

	GUID guid_num;
	record >> guid_num >> fSharedNote;
	guid = STR::guid2bstr(guid_num);
	record.skipNBytes(4);
}


} // namespace XLS

