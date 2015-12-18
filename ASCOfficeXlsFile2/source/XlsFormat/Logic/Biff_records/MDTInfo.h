#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/BiffString.h>

namespace XLS
{


// Logical representation of MDTInfo record in BIFF8
class MDTInfo: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(MDTInfo)
	BASE_OBJECT_DEFINE_CLASS_NAME(MDTInfo)
public:
	MDTInfo();
	~MDTInfo();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
//-----------------------------
	BIFF_BOOL fGhostRow;
	BIFF_BOOL fGhostCol;
	BIFF_BOOL fEdit;
	BIFF_BOOL fDelete;
	BIFF_BOOL fCopy;
	BIFF_BOOL fPasteAll;
	BIFF_BOOL fPasteFormulas;
	BIFF_BOOL fPasteValues;
	BIFF_BOOL fPasteFormats;
	BIFF_BOOL fPasteComments;
	BIFF_BOOL fPasteDataValidation;
	BIFF_BOOL fPasteBorders;
	BIFF_BOOL fPasteColWidths;
	BIFF_BOOL fPasteNumberFormats;
	BIFF_BOOL fMerge;
	BIFF_BOOL fSplitFirst;
	BIFF_BOOL fSplitAll;
	BIFF_BOOL fRowColShift;
	BIFF_BOOL fClearAll;
	BIFF_BOOL fClearFormats;
	BIFF_BOOL fClearContents;
	BIFF_BOOL fClearComments;
	BIFF_BOOL fAssign;
	BIFF_BOOL fCoerce;
	BIFF_BOOL fAdjust;
	BIFF_BOOL fCellMeta;

	LPWideString stName;

};

} // namespace XLS

