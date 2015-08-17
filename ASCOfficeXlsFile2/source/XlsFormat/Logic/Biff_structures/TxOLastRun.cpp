
#include "TxOLastRun.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr TxOLastRun::clone()
{
	return BiffStructurePtr(new TxOLastRun(*this));
}

TxOLastRun::TxOLastRun()
{		
}

TxOLastRun::~TxOLastRun()
{
}

//void TxOLastRun::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//	xml_tag->setAttribute(L"cchText", cchText);
//}
//
//void TxOLastRun::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}

void TxOLastRun::store(CFRecord& record)
{	
}

void TxOLastRun::load(CFRecord& record)
{
	record >> cchText;
	unsigned short _cchText = cchText;
	record.skipNunBytes(6);	// unused1 + unused2
}


} // namespace XLS

