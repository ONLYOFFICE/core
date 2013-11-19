#include "stdafx.h"
#include "MOper.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr MOper::clone()
{
	return BiffStructurePtr(new MOper(*this));
}


void MOper::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"colLast", colLast);
	own_tag->setAttribute(L"rowLast", rowLast);
	std::for_each(extOper.begin(), extOper.end(), boost::bind(&SerAr::toXML, _1, own_tag));
}


const bool MOper::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
#pragma message("####################### MOper record has no BiffStructure::fromXML() implemented")
	Log::error(" Error!!! MOper record has no BiffStructure::fromXML() implemented.");
	return false;
}


void MOper::store(CFRecord& record)
{
#pragma message("####################### MOper record has no BiffStructure::store() implemented")
	Log::error(" Error!!! MOper record has no BiffStructure::store() implemented.");
	//record << something;
}


void MOper::load(CFRecord& record)
{
	record >> colLast >> rowLast;
	for(int i = 0; i < (colLast + 1) * (rowLast + 1); ++i)
	{
		BYTE rec_type;
		record >> rec_type;
		SerArPtr ser(SerAr::createSerAr(rec_type));
		record >> *ser;
		extOper.push_back(ser);
	}
}


} // namespace XLS

