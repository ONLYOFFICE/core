#include "precompiled_xls.h"
#include "FrtHeader.h"
#include <Binary/CFRecord.h>

namespace XLS
{;

FrtHeader::FrtHeader()
{
}


FrtHeader::FrtHeader(const CFRecordType::TypeId type)
:	rt(type)
{
}


BiffStructurePtr FrtHeader::clone()
{
	return BiffStructurePtr(new FrtHeader(*this));
}


//void FrtHeader::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"rt", rt);
//	xml_tag->setAttribute(L"rt_name", CFRecordType::getStringById(rt).c_str());
//	xml_tag->setAttribute(L"fFrtRef", grbitFrt.fFrtRef);
//	xml_tag->setAttribute(L"fFrtAlert", grbitFrt.fFrtAlert);
//}
//
//
//void FrtHeader::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	throw;// EXCEPT::LE::WrongAPIUsage("FrtHeader must not get data from XML.", __FUNCTION__);
//}


void FrtHeader::store(CFRecord& record)
{
	record << rt << grbitFrt;
	record.reserveNunBytes(8); // reserved
}


void FrtHeader::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
	record.skipNunBytes(8); // reserved
}


} // namespace XLS

