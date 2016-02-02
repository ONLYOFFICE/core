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
	bool fGhostRow;
	bool fGhostCol;
	bool fEdit;
	bool fDelete;
	bool fCopy;
	bool fPasteAll;
	bool fPasteFormulas;
	bool fPasteValues;
	bool fPasteFormats;
	bool fPasteComments;
	bool fPasteDataValidation;
	bool fPasteBorders;
	bool fPasteColWidths;
	bool fPasteNumberFormats;
	bool fMerge;
	bool fSplitFirst;
	bool fSplitAll;
	bool fRowColShift;
	bool fClearAll;
	bool fClearFormats;
	bool fClearContents;
	bool fClearComments;
	bool fAssign;
	bool fCoerce;
	bool fAdjust;
	bool fCellMeta;

	LPWideString stName;

};

} // namespace XLS

