
#include "FrtRefHeaderU.h"
#include <Binary/CFRecord.h>
#include <Logic/Biff_structures/CellRangeRef.h>

namespace XLS
{;


BiffStructurePtr FrtRefHeaderU::clone()
{
	return BiffStructurePtr(new FrtRefHeaderU(*this));
}

//
//void FrtRefHeaderU::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"rt", rt);
//	xml_tag->setAttribute(L"rt_name", CFRecordType::getStringById(rt).c_str());
//	xml_tag->setAttribute(L"fFrtRef", grbitFrt.fFrtRef);
//	xml_tag->setAttribute(L"fFrtAlert", grbitFrt.fFrtAlert);
//	if(rt != rt_Feature11 && rt != rt_Feature12)
//	{
//		xml_tag->setAttribute(L"ref8", ref8);
//	}
//}
//
//
//void FrtRefHeaderU::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	throw;// EXCEPT::LE::WrongAPIUsage("FrtRefHeaderU must not get data from XML.", __FUNCTION__);
//}


void FrtRefHeaderU::store(CFRecord& record)
{
	record << rt << grbitFrt;
	record << ref8;
}


void FrtRefHeaderU::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
	record >> ref8;
}


} // namespace XLS

