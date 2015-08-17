
#include "Feat11FdaAutoFilter.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Feat11FdaAutoFilter::clone()
{
	return BiffStructurePtr(new Feat11FdaAutoFilter(*this));
}


//void Feat11FdaAutoFilter::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"cbAutoFilter", cbAutoFilter);
//	unsigned int size = cbAutoFilter;
//
//	if ( cbAutoFilter != 0 )
//	{
//		if (!recAutoFilter.m_bAutoFilter12)
//		{
//			if (recAutoFilter.doper1.vt == 0x06)
//				xml_tag->setAttribute("autoFilter_str1", recAutoFilter.str1.getEscaped_ST_Xstring());
//			if (recAutoFilter.doper2.vt == 0x06)
//				xml_tag->setAttribute("autoFilter_str2", recAutoFilter.str2.getEscaped_ST_Xstring());
//		}
//	}
//}
//
//
//void Feat11FdaAutoFilter::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}


void Feat11FdaAutoFilter::store(CFRecord& record)
{	
}


void Feat11FdaAutoFilter::load(CFRecord& record)
{	
	record >> cbAutoFilter;	
	record.skipNunBytes(2);	

	unsigned int size = cbAutoFilter;
	if (cbAutoFilter)
		recAutoFilter.readFields(record);
}

} // namespace XLS

