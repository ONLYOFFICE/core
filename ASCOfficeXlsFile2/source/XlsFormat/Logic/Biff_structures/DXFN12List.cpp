
#include "DXFN12List.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr DXFN12List::clone()
{
	return BiffStructurePtr(new DXFN12List(*this));
}

DXFN12List::DXFN12List()
{		
}


DXFN12List::~DXFN12List()
{
}


//void DXFN12List::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{		
//}
//
//
//void DXFN12List::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}


void DXFN12List::store(CFRecord& record)
{	
}


void DXFN12List::load(CFRecord& record)
{
	record >> dxfn;
	record >> xfext;
}


} // namespace XLS

