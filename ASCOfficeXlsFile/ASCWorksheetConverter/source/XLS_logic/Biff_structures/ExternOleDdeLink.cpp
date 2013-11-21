#include "stdafx.h"
#include "ExternOleDdeLink.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr ExternOleDdeLink::clone()
{
	return BiffStructurePtr(new ExternOleDdeLink(*this));
}


void ExternOleDdeLink::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"lStgName", lStgName);
	own_tag->setAttribute(L"linkName", linkName);
	moper.toXML(own_tag);
}


const bool ExternOleDdeLink::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### ExternOleDdeLink record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! ExternOleDdeLink record has no BiffStructure::fromXML() implemented.");
	return false;
}


void ExternOleDdeLink::store(CFRecord& record)
{
#pragma message("####################### ExternOleDdeLink record has no BiffStructure::store() implemented")
	Log::error(" Error!!! ExternOleDdeLink record has no BiffStructure::store() implemented.");
	//record << something;
}


void ExternOleDdeLink::load(CFRecord& record)
{
	record >> lStgName >> linkName;
	if(!record.isEOF())
	{
		record >> moper;
	}
}


} // namespace XLS

