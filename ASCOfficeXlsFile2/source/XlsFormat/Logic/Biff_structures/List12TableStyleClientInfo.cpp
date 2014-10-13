#include "precompiled_xls.h"
#include "List12TableStyleClientInfo.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr List12TableStyleClientInfo::clone()
{
	return BiffStructurePtr(new List12TableStyleClientInfo(*this));
}

List12TableStyleClientInfo::List12TableStyleClientInfo()
{		
}


List12TableStyleClientInfo::~List12TableStyleClientInfo()
{
}

//
//void List12TableStyleClientInfo::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{		
//	xml_tag->setAttribute(L"fFirstColumn", fFirstColumn);
//	xml_tag->setAttribute(L"fLastColumn", fLastColumn);
//	xml_tag->setAttribute(L"fRowStripes", fRowStripes);
//	xml_tag->setAttribute(L"fColumnStripes", fColumnStripes);
//	xml_tag->setAttribute(L"fDefaultStyle", fDefaultStyle);
//	xml_tag->setAttribute(L"stListStyleName", stListStyleName);
//}

//
//void List12TableStyleClientInfo::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}


void List12TableStyleClientInfo::store(CFRecord& record)
{	
}


void List12TableStyleClientInfo::load(CFRecord& record)
{	
	unsigned __int16 flags;
	record >> flags;

	fFirstColumn = static_cast<unsigned char>(GETBITS(flags, 0, 1));
	fLastColumn = static_cast<unsigned char>(GETBITS(flags, 1, 2));
	fRowStripes = static_cast<unsigned char>(GETBITS(flags, 3, 4));
	fColumnStripes = static_cast<unsigned char>(GETBITS(flags, 5, 6));
	fDefaultStyle  = static_cast<unsigned char>(GETBITS(flags, 9, 10));

	record >> stListStyleName;
}


} // namespace XLS

