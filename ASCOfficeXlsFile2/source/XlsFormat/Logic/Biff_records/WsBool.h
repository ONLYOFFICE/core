#pragma once

#include "BiffRecord.h"

namespace XLS
{;


// Logical representation of WsBool record in BIFF8
class WsBool: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(WsBool)
	BASE_OBJECT_DEFINE_CLASS_NAME(WsBool)
public:
	WsBool(const bool is_dialog_sheet);
	~WsBool();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);

	static const ElementType	type = typeWsBool;

//-----------------------------
	BIFF_BOOL fShowAutoBreaks;
	BIFF_BOOL fDialog;
	BIFF_BOOL fApplyStyles;
	BIFF_BOOL fRowSumsBelow;
	BIFF_BOOL fColSumsRight;
	BIFF_BOOL fFitToPage;
	BIFF_BOOL fDspGuts;
	BIFF_BOOL fSyncHoriz;
	BIFF_BOOL fSyncVert;
	BIFF_BOOL fAltExprEval;
	BIFF_BOOL fAltFormulaEntry;


};

} // namespace XLS

