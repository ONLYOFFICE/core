
#include "FtMacro.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


FtMacro::FtMacro()
:	fmla(false)
{
}

BiffStructurePtr FtMacro::clone()
{
	return BiffStructurePtr(new FtMacro(*this));
}

//
//void FtMacro::setXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmla.toXML(own_tag);
//}
//
//
//void FtMacro::getXMLAttributes(MSXML2::IXMLDOMElementPtr own_tag)
//{
//	fmla.fromXML(own_tag);
//}


void FtMacro::store(CFRecord& record)
{
	unsigned short ft = 0x0004; // reserved
	record << ft;

	fmla.store(record);
}


void FtMacro::load(CFRecord& record)
{
	record.skipNunBytes(2); // reserved

	fmla.load(record);
}


} // namespace XLS

