
#include "RevNameTabid.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


RevNameTabid::RevNameTabid()
{
}


RevNameTabid::RevNameTabid(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevNameTabid::clone()
{
	return BiffStructurePtr(new RevNameTabid(*this));
}


//void RevNameTabid::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"tabid", tabid);
//	name.toXML(xml_tag);
//}


void RevNameTabid::store(CFRecord& record)
{
	record << tabid << name;
}


void RevNameTabid::load(CFRecord& record)
{
	record >> tabid >> name;
}


const std::wstring RevNameTabid::toString() const
{
	return name.toString();
}

} // namespace XLS

