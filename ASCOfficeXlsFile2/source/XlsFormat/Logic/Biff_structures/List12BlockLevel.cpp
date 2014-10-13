#include "precompiled_xls.h"
#include "List12BlockLevel.h"
#include <Binary/CFRecord.h>

namespace XLS
{;


BiffStructurePtr List12BlockLevel::clone()
{
	return BiffStructurePtr(new List12BlockLevel(*this));
}

List12BlockLevel::List12BlockLevel()
{		
}


List12BlockLevel::~List12BlockLevel()
{
}


//void List12BlockLevel::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//	xml_tag->setAttribute(L"cbdxfHeader", cbdxfHeader);
//	xml_tag->setAttribute(L"cbdxfData", cbdxfData);
//	xml_tag->setAttribute(L"cbdxfAgg", cbdxfAgg);
//	xml_tag->setAttribute(L"cbdxfBorder", cbdxfBorder);
//	xml_tag->setAttribute(L"cbdxfHeaderBorder", cbdxfHeaderBorder);
//	xml_tag->setAttribute(L"cbdxfAggBorder", cbdxfAggBorder);
//	xml_tag->setAttribute(L"istnHeader", istnHeader);
//	xml_tag->setAttribute(L"istnData", istnData);
//	xml_tag->setAttribute(L"istnAgg", istnAgg);
//
//	/*if (cbdxfHeader)
//		dxfHeader.toXML(xml_tag);;
//	if (cbdxfData)
//		dxfData.toXML(xml_tag);;
//	if (cbdxfAgg)
//		dxfAgg.toXML(xml_tag);;
//	if (cbdxfBorder)
//		dxfBorder.toXML(xml_tag);;
//	if (cbdxfHeaderBorder)
//		dxfHeaderBorder.toXML(xml_tag);;
//	if (cbdxfAggBorder)
//		dxfAggBorder.toXML(xml_tag);;
//	if (istnHeader != -1)
//		stHeader.toXML(xml_tag);;
//	if (istnData != -1)
//		stData.toXML(xml_tag);;
//	if (istnAgg != -1)
//		stAgg.toXML(xml_tag);;*/
//}

//
//void List12BlockLevel::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}
//

void List12BlockLevel::store(CFRecord& record)
{	
}


void List12BlockLevel::load(CFRecord& record)
{	
	record >> cbdxfHeader;
	record >> istnHeader;
	record >> cbdxfData;
	record >> istnData;
	record >> cbdxfAgg;
	record >> istnAgg;
	record >> cbdxfBorder;
	record >> cbdxfHeaderBorder;
	record >> cbdxfAggBorder;
	
	if (cbdxfHeader)
		record >> dxfHeader;
	if (cbdxfData)
		record >> dxfData;
	if (cbdxfAgg)
		record >> dxfAgg;
	if (cbdxfBorder)
		record >> dxfBorder;
	if (cbdxfHeaderBorder)
		record >> dxfHeaderBorder;
	if (cbdxfAggBorder)
		record >> dxfAggBorder;
	if (istnHeader != -1)
		record >> stHeader;
	if (istnData != -1)
		record >> stData;
	if (istnAgg != -1)
		record >> stAgg;
}


} // namespace XLS

