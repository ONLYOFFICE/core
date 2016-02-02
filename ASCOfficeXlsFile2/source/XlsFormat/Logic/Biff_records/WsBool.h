#pragma once

#include "BiffRecord.h"

namespace XLS
{


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
	bool fShowAutoBreaks;
	bool fDialog;
	bool fApplyStyles;
	bool fRowSumsBelow;
	bool fColSumsRight;
	bool fFitToPage;
	bool fDspGuts;
	bool fSyncHoriz;
	bool fSyncVert;
	bool fAltExprEval;
	bool fAltFormulaEntry;


};

} // namespace XLS

