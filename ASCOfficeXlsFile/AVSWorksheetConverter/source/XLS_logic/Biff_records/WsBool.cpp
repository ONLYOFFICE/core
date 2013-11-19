#include "stdafx.h"
#include "WsBool.h"

namespace XLS
{;

WsBool::WsBool(const bool is_dialog_sheet)
:	fDialog(is_dialog_sheet)
{
}


WsBool::~WsBool()
{
}


BaseObjectPtr WsBool::clone()
{
	return BaseObjectPtr(new WsBool(*this));
}


void WsBool::writeFields(CFRecord& record)
{
	WORD flags = 0;
	SETBIT(flags, 0, fShowAutoBreaks);
	SETBIT(flags, 4, fDialog);
	SETBIT(flags, 5, fApplyStyles);
	SETBIT(flags, 6, fRowSumsBelow);
	SETBIT(flags, 7, fColSumsRight);
	SETBIT(flags, 8, fFitToPage);
	SETBIT(flags, 10, fDspGuts);
	SETBIT(flags, 12, fSyncHoriz);
	SETBIT(flags, 13, fSyncVert);
	SETBIT(flags, 14, fAltExprEval);
	SETBIT(flags, 15, fAltFormulaEntry);
	record << flags;
}


void WsBool::readFields(CFRecord& record)
{
	WORD flags;
	record >> flags;
	fShowAutoBreaks = GETBIT(flags, 0);
	fDialog = GETBIT(flags, 4);
	fApplyStyles = GETBIT(flags, 5);
	fRowSumsBelow = GETBIT(flags, 6);
	fColSumsRight = GETBIT(flags, 7);
	fFitToPage = GETBIT(flags, 8);
	fDspGuts = GETBIT(flags, 10);
	fSyncHoriz = GETBIT(flags, 12);
	fSyncVert = GETBIT(flags, 13);
	fAltExprEval = GETBIT(flags, 14);
	fAltFormulaEntry = GETBIT(flags, 15);
}


} // namespace XLS

