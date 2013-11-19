#include "stdafx.h"
#include "List12DisplayName.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


BiffStructurePtr List12DisplayName::clone()
{
	return BiffStructurePtr(new List12DisplayName(*this));
}

List12DisplayName::List12DisplayName()
{		
}


List12DisplayName::~List12DisplayName()
{
}


void List12DisplayName::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
	xml_tag->setAttribute(L"stListName", stListName);
	xml_tag->setAttribute(L"stListComment", stListComment);
}


void List12DisplayName::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{	
}


void List12DisplayName::store(CFRecord& record)
{	
}


void List12DisplayName::load(CFRecord& record)
{	
	record >> stListName;
	record >> stListComment;
}


} // namespace XLS

