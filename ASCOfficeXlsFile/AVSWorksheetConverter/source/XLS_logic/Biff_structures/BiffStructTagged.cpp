#include "stdafx.h"
#include "BiffStructTagged.h"
#include <XLS_bin/CFRecord.h>
#include <Exception/UnexpectedProgramPath.h>
#include <Exception/AttributeNotFound.h>
#include <Exception/ChildNodeNotFound.h>

namespace XLS
{;


// This is a stub for that descendants of BiffStructure those are not nested from BiffStructTagged
void BiffStructTagged::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);
	setXMLAttributes(own_tag);
}


// This is a stub for that descendants of BiffStructure those are not nested from BiffStructTagged
const bool BiffStructTagged::fromXML(MSXML2::IXMLDOMElementPtr parent, const size_t position, const bool is_mandatory)
{
	MSXML2::IXMLDOMNodeListPtr nodes = parent->selectNodes(getClassName());
	MSXML2::IXMLDOMElementPtr own_tag;
	for(size_t counter = 1; (own_tag = nodes->nextNode()); ++counter)
	{
		if(counter == position)
		{
			getXMLAttributes(own_tag);
			return true;
		}
	}

	if(is_mandatory)
	{
		throw EXCEPT::LE::ChildNodeNotFound(static_cast<char*>(getClassName()), static_cast<char*>(parent->GetnodeName()));
	}
	return false;
}


}// namespace XLS
