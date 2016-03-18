
#include "Feat11FieldDataItem.h"
#include <Binary/CFRecord.h>

namespace XLS
{


BiffStructurePtr Feat11FieldDataItem::clone()
{
	return BiffStructurePtr(new Feat11FieldDataItem(*this));
}



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

	_UINT32 flags;
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

