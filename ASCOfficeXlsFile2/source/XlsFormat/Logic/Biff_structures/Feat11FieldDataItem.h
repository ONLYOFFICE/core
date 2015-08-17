#pragma once

#include "BiffStructure.h"
#include <Logic/Biff_structures/BiffString.h>
#include <Logic/Biff_structures/DXFN12List.h>
#include <Logic/Biff_structures/Feat11FdaAutoFilter.h>

namespace XLS
{

class CFRecord;

class Feat11FieldDataItem : public BiffStructure
{
	BASE_STRUCTURE_DEFINE_CLASS_NAME(Feat11FieldDataItem)
public:
	BiffStructurePtr clone();

	static const ElementType	type = typeFeat11FieldDataItem;
	
	virtual void load(CFRecord& record);
	virtual void store(CFRecord& record);

	BIFF_DWORD idField;
	BIFF_DWORD lfdt;
	BIFF_DWORD lfxidt;
	BIFF_DWORD ilta;
	BIFF_DWORD cbFmtAgg;
	BIFF_DWORD istnAgg;

	BIFF_BYTE fAutoFilter;
	BIFF_BYTE fAutoFilterHidden;
	BIFF_BYTE fLoadXmapi;
	BIFF_BYTE fLoadFmla;
	BIFF_BYTE fLoadTotalFmla;
	BIFF_BYTE fLoadTotalArray;
	BIFF_BYTE fSaveStyleName;
	BIFF_BYTE fLoadTotalStr;
	BIFF_BYTE fAutoCreateCalcCol;
	
	BIFF_DWORD cbFmtInsertRow;
	BIFF_DWORD istnInsertRow;

	XLUnicodeString strFieldName;
	XLUnicodeString strCaption;

	DXFN12List dxfFmtAgg;
	DXFN12List dxfFmtInsertRow;

	Feat11FdaAutoFilter AutoFilter;
};

typedef boost::shared_ptr<Feat11FieldDataItem> Feat11FieldDataItemPtr;

} // namespace XLS

