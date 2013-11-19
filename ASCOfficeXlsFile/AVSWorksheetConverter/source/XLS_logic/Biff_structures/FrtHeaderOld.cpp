#include "stdafx.h"
#include "FrtHeaderOld.h"
#include <XLS_bin/CFRecord.h>

namespace XLS
{;


FrtHeaderOld::FrtHeaderOld()
{
}

FrtHeaderOld::FrtHeaderOld(const CFRecordType::TypeId type)
:	rt(type)
{
}

BiffStructurePtr FrtHeaderOld::clone()
{
	return BiffStructurePtr(new FrtHeaderOld(*this));
}


void FrtHeaderOld::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	xml_tag->setAttribute(L"rt", rt);
	xml_tag->setAttribute(L"rt_name", CFRecordType::getStringById(rt).c_str());
	xml_tag->setAttribute(L"fFrtRef", grbitFrt.fFrtRef);
	xml_tag->setAttribute(L"fFrtAlert", grbitFrt.fFrtAlert);
}


void FrtHeaderOld::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
{
	throw EXCEPT::LE::WrongAPIUsage("FrtRefHeaderU must not get data from XML.", __FUNCTION__);
}


void FrtHeaderOld::store(CFRecord& record)
{
	grbitFrt.fFrtRef = 0;
	grbitFrt.fFrtAlert = 0;
	record << rt << grbitFrt;
}


void FrtHeaderOld::load(CFRecord& record)
{
	record >> rt >> grbitFrt;
}


} // namespace XLS

