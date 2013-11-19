#include "stdafx.h"
#include "FileMoniker.h"
#include <XLS_bin/CFRecord.h>

namespace OSHARED
{;


FileMoniker::FileMoniker()
{
}


FileMoniker::FileMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr FileMoniker::clone()
{
	return XLS::BiffStructurePtr(new FileMoniker(*this));
}


void FileMoniker::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"cAnti", cAnti);
	xml_tag->setAttribute(L"ansiPath", ansiPath.c_str());
	xml_tag->setAttribute(L"endServer", endServer);
	xml_tag->setAttribute(L"versionNumber", versionNumber);
	if(cbUnicodePathSize)
	{
		xml_tag->setAttribute(L"usKeyValue", usKeyValue);
		xml_tag->setAttribute(L"unicodePath", STR::escapeUrlW(unicodePath).c_str());
	}
}


void FileMoniker::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
#pragma message("####################### FileMoniker record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! FileMoniker record has no BiffStructure::fromXML() implemented.");
}


void FileMoniker::store(XLS::CFRecord& record)
{
#pragma message("####################### FileMoniker record has no BiffStructure::store() implemented")
	Log::error(" Error!!! FileMoniker record has no BiffStructure::store() implemented.");
	//record << something;
}


void FileMoniker::load(XLS::CFRecord& record)
{
	DWORD ansiLength;
	record >> cAnti >> ansiLength >> ansiPath >> endServer >> versionNumber;
	record.skipNBytes(20); // reserved
	record >> cbUnicodePathSize;
	if(cbUnicodePathSize)
	{
		DWORD cbUnicodePathBytes;
		record >> cbUnicodePathBytes >> usKeyValue;
		unicodePath = std::wstring(record.getCurData<wchar_t>(), cbUnicodePathBytes / 2);
		record.skipNBytes(cbUnicodePathBytes);
	}
}


} // namespace OSHARED

