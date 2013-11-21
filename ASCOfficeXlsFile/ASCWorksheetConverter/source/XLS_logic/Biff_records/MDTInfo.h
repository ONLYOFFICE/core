#pragma once

#include "BiffRecord.h"
#include <XLS_logic/Biff_structures/BiffString.h>

namespace XLS
{;


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
private:
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
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fGhostRow)
		BO_ATTRIB_MARKUP_ATTRIB(fGhostCol)
		BO_ATTRIB_MARKUP_ATTRIB(fEdit)
		BO_ATTRIB_MARKUP_ATTRIB(fDelete)
		BO_ATTRIB_MARKUP_ATTRIB(fCopy)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteAll)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteFormulas)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteValues)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteFormats)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteComments)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteDataValidation)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteBorders)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteColWidths)
		BO_ATTRIB_MARKUP_ATTRIB(fPasteNumberFormats)
		BO_ATTRIB_MARKUP_ATTRIB(fMerge)
		BO_ATTRIB_MARKUP_ATTRIB(fSplitFirst)
		BO_ATTRIB_MARKUP_ATTRIB(fSplitAll)
		BO_ATTRIB_MARKUP_ATTRIB(fRowColShift)
		BO_ATTRIB_MARKUP_ATTRIB(fClearAll)
		BO_ATTRIB_MARKUP_ATTRIB(fClearFormats)
		BO_ATTRIB_MARKUP_ATTRIB(fClearContents)
		BO_ATTRIB_MARKUP_ATTRIB(fClearComments)
		BO_ATTRIB_MARKUP_ATTRIB(fAssign)
		BO_ATTRIB_MARKUP_ATTRIB(fCoerce)
		BO_ATTRIB_MARKUP_ATTRIB(fAdjust)
		BO_ATTRIB_MARKUP_ATTRIB(fCellMeta)

		BO_ATTRIB_MARKUP_ATTRIB(stName)
	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

