#include "precompiled_xls.h"
#include "MDTInfo.h"

namespace XLS
{;

MDTInfo::MDTInfo()
{
}


MDTInfo::~MDTInfo()
{
}


BaseObjectPtr MDTInfo::clone()
{
	return BaseObjectPtr(new MDTInfo(*this));
}


void MDTInfo::writeFields(CFRecord& record)
{
#pragma message("####################### MDTInfo record is not implemented")
	Log::error("MDTInfo record is not implemented.");
	//record << some_value;
}


void MDTInfo::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	unsigned int flags;
	record >> flags;
	fGhostRow = GETBIT(flags, 0);
	fGhostCol = GETBIT(flags, 1);
	fEdit = GETBIT(flags, 2);
	fDelete = GETBIT(flags, 3);
	fCopy = GETBIT(flags, 4);
	fPasteAll = GETBIT(flags, 5);
	fPasteFormulas = GETBIT(flags, 6);
	fPasteValues = GETBIT(flags, 7);
	fPasteFormats = GETBIT(flags, 8);
	fPasteComments = GETBIT(flags, 9);
	fPasteDataValidation = GETBIT(flags, 10);
	fPasteBorders = GETBIT(flags, 11);
	fPasteColWidths = GETBIT(flags, 12);
	fPasteNumberFormats = GETBIT(flags, 13);
	fMerge = GETBIT(flags, 14);
	fSplitFirst = GETBIT(flags, 15);
	fSplitAll = GETBIT(flags, 16);
	fRowColShift = GETBIT(flags, 17);
	fClearAll = GETBIT(flags, 18);
	fClearFormats = GETBIT(flags, 19);
	fClearContents = GETBIT(flags, 20);
	fClearComments = GETBIT(flags, 21);
	fAssign = GETBIT(flags, 22);
	fCoerce = GETBIT(flags, 28);
	fAdjust = GETBIT(flags, 29);
	fCellMeta = GETBIT(flags, 30);

	record >> stName;
}

} // namespace XLS

