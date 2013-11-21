#include "stdafx.h"
#include "FrtRefHeaderNoGrbit.h"
#include <XLS_bin/CFRecord.h>
#include <XLS_logic/Biff_structures/CellRangeRef.h>
#include <Exception/WrongAPIUsage.h>

namespace XLS
{;


FrtRefHeaderNoGrbit::FrtRefHeaderNoGrbit(const CFRecordType::TypeId type)
:	rt(type)
{
}


FrtRefHeaderNoGrbit::FrtRefHeaderNoGrbit(const CFRecordType::TypeId type, const Ref8U& ref8_init)
:	rt(type),
	ref8(ref8_init)
{
}


BiffStructurePtr FrtRefHeaderNoGrbit::clone()
{
	return BiffStructurePtr(new FrtRefHeaderNoGrbit(*this));
}


void FrtRefHeaderNoGrbit::toXML(MSXML2::IXMLDOMElementPtr parent)
{
	MSXML2::IXMLDOMElementPtr own_tag = XMLSTUFF::makeXMLNode(getClassName(), parent);

	own_tag->setAttribute(L"rt", rt);
	own_tag->setAttribute(L"rt_name", CFRecordType::getStringById(rt).c_str());
}


const bool FrtRefHeaderNoGrbit::fromXML(MSXML2::IXMLDOMElementPtr parent)
{
	throw EXCEPT::LE::WrongAPIUsage("FrtRefHeaderNoGrbit must not get data from XML.", __FUNCTION__);
}


void FrtRefHeaderNoGrbit::store(CFRecord& record)
{
	record << rt;
	record << ref8;
}


void FrtRefHeaderNoGrbit::load(CFRecord& record)
{
	record >> rt;
	record >> ref8;
}


} // namespace XLS

