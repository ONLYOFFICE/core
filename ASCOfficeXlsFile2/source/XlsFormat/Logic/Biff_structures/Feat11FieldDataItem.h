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

	_UINT32	idField;
	_UINT32	lfdt;
	_UINT32	lfxidt;
	_UINT32	ilta;
	_UINT32	cbFmtAgg;
	_UINT32	istnAgg;

	unsigned char	fAutoFilter;
	unsigned char	fAutoFilterHidden;
	unsigned char	fLoadXmapi;
	unsigned char	fLoadFmla;
	unsigned char	fLoadTotalFmla;
	unsigned char	fLoadTotalArray;
	unsigned char	fSaveStyleName;
	unsigned char	fLoadTotalStr;
	unsigned char	fAutoCreateCalcCol;
	
	_UINT32	cbFmtInsertRow;
	_UINT32	istnInsertRow;

	XLUnicodeString strFieldName;
	XLUnicodeString strCaption;

	DXFN12List dxfFmtAgg;
	DXFN12List dxfFmtInsertRow;

	Feat11FdaAutoFilter AutoFilter;
};

typedef boost::shared_ptr<Feat11FieldDataItem> Feat11FieldDataItemPtr;

} // namespace XLS

