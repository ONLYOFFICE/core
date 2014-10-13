#include "precompiled_xls.h"
#include "OfficeArtTertiaryFOPT.h"
#include <Binary/CFRecord.h>

namespace ODRAW
{;


OfficeArtTertiaryFOPT::OfficeArtTertiaryFOPT()
:	OfficeArtRecord(0x03, TertiaryFOPT)
{
}


XLS::BiffStructurePtr OfficeArtTertiaryFOPT::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtTertiaryFOPT(*this));
}


//void OfficeArtTertiaryFOPT::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	fopt.toXML(xml_tag);
//}
//
//
//void OfficeArtTertiaryFOPT::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	fopt.fromXML(xml_tag);
//}


void OfficeArtTertiaryFOPT::storeFields(XLS::CFRecord& record)
{
	record << fopt;
}


void OfficeArtTertiaryFOPT::loadFields(XLS::CFRecord& record)
{
	fopt.SetOptionsNumber(rh_own.recInstance);
	record >> fopt;
}


const unsigned __int16 OfficeArtTertiaryFOPT::GetInstanceToStore()
{
	return fopt.GetOptionsNumber();
}


} // namespace XLS
