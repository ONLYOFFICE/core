#include "stdafx.h"
#include "DropDownObjId.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DropDownObjId::clone()
{
	return BiffStructurePtr(new DropDownObjId(*this));
}


void DropDownObjId::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"id", id);
}


const bool DropDownObjId::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = parent->selectSingleNode(getClassName());
	if(!own_tag)
	{
		return false;
	}
	id = getStructAttribute(own_tag, L"id");
	return true;
}


void DropDownObjId::store(CFRecord& record)
{
	record << id;
}


void DropDownObjId::load(CFRecord& record)
{
	record >> id;
}


} // namespace XLS

