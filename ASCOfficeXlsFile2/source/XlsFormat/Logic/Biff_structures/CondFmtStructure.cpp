#include "precompiled_xls.h"
#include "CondFmtStructure.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr CondFmtStructure::clone()
{
	return BiffStructurePtr(new CondFmtStructure(*this));
}


//void CondFmtStructure::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"ccf", ccf);
//	xml_tag->setAttribute(L"fToughRecalc", fToughRecalc);
//	xml_tag->setAttribute(L"nID", nID);
//	xml_tag->setAttribute(L"refBound", refBound);
//	sqref.toXML(xml_tag);
//}
//
//
//void CondFmtStructure::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	ccf = getStructAttribute(xml_tag, L"ccf");
//	fToughRecalc = getStructAttribute(xml_tag, L"fToughRecalc");
//	nID = getStructAttribute(xml_tag, L"nID");
//	refBound = static_cast<std::wstring >(getStructAttribute(xml_tag, L"refBound"));
//	sqref.toXML(xml_tag);
//}


void CondFmtStructure::store(CFRecord& record)
{
	record << ccf;
	unsigned short flags = 0;
	SETBIT(flags, 0, fToughRecalc);
	SETBITS(flags, 1, 15, nID);
	record << flags;
	record << refBound << sqref;
}


void CondFmtStructure::load(CFRecord& record)
{
	record >> ccf;
	unsigned short flags;
	record >> flags;
	fToughRecalc = GETBIT(flags, 0);
	nID = GETBITS(flags, 1, 15);
	record >> refBound >> sqref;
}


const CellRef CondFmtStructure::getLocation() const
{
	return sqref.getLocationFirstCell();
}

} // namespace XLS

