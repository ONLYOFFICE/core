
#include "Feat11FieldDataItem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Feat11FieldDataItem::clone()
{
	return BiffStructurePtr(new Feat11FieldDataItem(*this));
}


//void Feat11FieldDataItem::setXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{
//	xml_tag->setAttribute(L"idField", idField);
//	xml_tag->setAttribute(L"lfdt", lfdt);
//	xml_tag->setAttribute(L"lfxidt", lfxidt);
//	xml_tag->setAttribute(L"ilta", ilta);
//	xml_tag->setAttribute(L"cbFmtAgg", cbFmtAgg);
//	xml_tag->setAttribute(L"istnAgg", istnAgg);
//
//	xml_tag->setAttribute(L"fAutoFilter", fAutoFilter);
//	xml_tag->setAttribute(L"fAutoFilterHidden", fAutoFilterHidden);
//	xml_tag->setAttribute(L"fLoadXmapi", fLoadXmapi);
//	xml_tag->setAttribute(L"fLoadFmla", fLoadFmla);
//	xml_tag->setAttribute(L"fLoadTotalFmla", fLoadTotalFmla);
//	xml_tag->setAttribute(L"fLoadTotalArray", fLoadTotalArray);
//	xml_tag->setAttribute(L"fSaveStyleName", fSaveStyleName);
//	xml_tag->setAttribute(L"fLoadTotalStr", fLoadTotalStr);
//	xml_tag->setAttribute(L"fAutoCreateCalcCol", fAutoCreateCalcCol);
//
//	xml_tag->setAttribute(L"cbFmtInsertRow", cbFmtInsertRow);	
//	xml_tag->setAttribute(L"istnInsertRow", istnInsertRow);	
//	xml_tag->setAttribute(L"strFieldName", strFieldName);	
//	xml_tag->setAttribute(L"strCaption", strCaption);	
//
//	if (cbFmtAgg > 0)
//		dxfFmtAgg.toXML(xml_tag);
//	if (cbFmtInsertRow > 0)
//		dxfFmtInsertRow.toXML(xml_tag);
//	if (fAutoFilter == 1)
//		AutoFilter.toXML(xml_tag);
//}
//
//
//void Feat11FieldDataItem::getXMLAttributes(MSXML2::IXMLDOMElementPtr xml_tag)
//{	
//}


void Feat11FieldDataItem::store(CFRecord& record)
{	
}


void Feat11FieldDataItem::load(CFRecord& record)
{	

	record >> idField;
	record >> lfdt;
	record >> lfxidt;
	record >> ilta;
	record >> cbFmtAgg;
	record >> istnAgg;

	unsigned int flags;
	record >> flags;

	fAutoFilter = static_cast<unsigned char>(GETBIT(flags, 0));
	fAutoFilterHidden = static_cast<unsigned char>(GETBIT(flags, 1));
	fLoadXmapi = static_cast<unsigned char>(GETBIT(flags, 2));
	fLoadFmla = static_cast<unsigned char>(GETBIT(flags, 3));	
	fLoadTotalFmla = static_cast<unsigned char>(GETBIT(flags, 7));
	fLoadTotalArray = static_cast<unsigned char>(GETBIT(flags, 8));
	fSaveStyleName = static_cast<unsigned char>(GETBIT(flags, 9));
	fLoadTotalStr = static_cast<unsigned char>(GETBIT(flags, 10));
	fAutoCreateCalcCol = static_cast<unsigned char>(GETBIT(flags, 11));

	record >> cbFmtInsertRow;
	record >> istnInsertRow;
	record >> strFieldName;
	record >> strCaption;

	if (record.getRdPtr() >= record.getDataSize())
		return;//125 Planilhas de Excel.xls

	if (cbFmtAgg > 0)
		record >> dxfFmtAgg;
	if (cbFmtInsertRow > 0)
		record >> dxfFmtInsertRow;
	if (fAutoFilter == BIFF_BYTE(1))
		record >> AutoFilter;
}

} // namespace XLS

