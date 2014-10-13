#include "precompiled_xls.h"
#include "FrtRefHeader.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr FrtRefHeader::clone()
{
	return BiffStructurePtr(new FrtRefHeader(*this));
}

//
//void FrtRefHeader::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"rt", rt);
//	xml_tag->setAttribute(L"rt_name", CFRecordType::getStringById(rt).c_str());
//	xml_tag->setAttribute(L"fFrtRef", grbitFrt.fFrtRef);
//	xml_tag->setAttribute(L"fFrtAlert", grbitFrt.fFrtAlert);
//	if(grbitFrt.fFrtRef)
//	{
//		xml_tag->setAttribute(L"ref8", ref8);
//	}
//}
//
//
//void FrtRefHeader::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	throw;// EXCEPT::LE::WrongAPIUsage("FrtRefHeaderU must not get data from XML.", __FUNCTION__);
//}


void FrtRefHeader::store(CFRecord& record)
{
	record << rt << grbitFrt;
	record << ref8;
}


void FrtRefHeader::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
	record >> ref8;
}


} // namespace XLS

