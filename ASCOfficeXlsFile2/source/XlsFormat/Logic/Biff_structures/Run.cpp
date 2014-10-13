#include "precompiled_xls.h"
#include "Run.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr Run::clone()
{
	return BiffStructurePtr(new Run(*this));
}

Run::Run()
{		
}

Run::~Run()
{
}
//
//void Run::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//	formatRun.toXML(xml_tag);
//}
//
//void Run::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}

void Run::store(CFRecord& record)
{	
}

void Run::load(CFRecord& record)
{
	formatRun.load(record);
	record.skipNunBytes(4);	// unused1 + unused2
}


} // namespace XLS

