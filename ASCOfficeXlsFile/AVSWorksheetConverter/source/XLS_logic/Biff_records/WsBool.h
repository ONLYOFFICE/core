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

private:
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
public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(fShowAutoBreaks)
		BO_ATTRIB_MARKUP_ATTRIB(fDialog)
		BO_ATTRIB_MARKUP_ATTRIB(fApplyStyles)
		BO_ATTRIB_MARKUP_ATTRIB(fRowSumsBelow)
		BO_ATTRIB_MARKUP_ATTRIB(fColSumsRight)
		BO_ATTRIB_MARKUP_ATTRIB(fFitToPage)
		BO_ATTRIB_MARKUP_ATTRIB(fDspGuts)
		BO_ATTRIB_MARKUP_ATTRIB(fSyncHoriz)
		BO_ATTRIB_MARKUP_ATTRIB(fSyncVert)
		BO_ATTRIB_MARKUP_ATTRIB(fAltExprEval)
		BO_ATTRIB_MARKUP_ATTRIB(fAltFormulaEntry)
/*
		IXMLDOMElementPtr ss_node = XMLSTUFF::findElementAmongParents(proc.getParent(), "WorksheetSubstream");
		if(ss_node)
		{
			ss_node->setAttribute(L"isDialog", _variant_t(fDialog)); // If find a way how to change the name, do it instead of setting an attribute
		}
*/

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

