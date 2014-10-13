#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/PaneType.h>

namespace XLS
{;


// Logical representation of UserSViewBegin record in BIFF8
class UserSViewBegin: public BiffRecord
{
	BIFF_RECORD_DEFINE_TYPE_INFO(UserSViewBegin)
	BASE_OBJECT_DEFINE_CLASS_NAME(UserSViewBegin)
public:
	UserSViewBegin();
	~UserSViewBegin();

	BaseObjectPtr clone();

	void writeFields(CFRecord& record);
	void readFields(CFRecord& record);
private:
	BIFF_BSTR guid;
	TabId iTabid;
	BIFF_DWORD wScale;
	Icv icvHdr;
	PaneType pnnSel;

	BIFF_BOOL fShowBrks;
	BIFF_BOOL fDspFmlaSv;
	BIFF_BOOL fDspGridSv;
	BIFF_BOOL fDspRwColSv;
	BIFF_BOOL fDspGutsSv;
	BIFF_BOOL fDspZerosSv;
	BIFF_BOOL fHorizontal;
	BIFF_BOOL fVertical;
	BIFF_BOOL fPrintRwCol;
	BIFF_BOOL fPrintGrid;
	BIFF_BOOL fFitToPage;
	BIFF_BOOL fPrintArea;
	BIFF_BOOL fOnePrintArea;
	BIFF_BOOL fFilterMode;
	BIFF_BOOL fEzFilter;
	BIFF_BOOL fFrozen;

	BIFF_BOOL fFrozenNoSplit;
	BIFF_BOOL fSplitV;
	BIFF_BOOL fSplitH;
	BIFF_BOOL fHiddenRw;
	BIFF_BOOL fHiddenCol;
	BIFF_BOOL fFilterUnique;
	BIFF_BOOL fSheetLayoutView;
	BIFF_BOOL fPageLayoutView;
	BIFF_BOOL fRuler;

	BIFF_BSTR ref8TopLeft;
	BIFF_DOUBLE operNumX;
	BIFF_DOUBLE operNumY;
	ForwardOnlyParam<unsigned __int16> colRPane;
	ForwardOnlyParam<unsigned __int16> rwBPane;

	BIFF_BSTR pane_top_left_cell;

public:
	BO_ATTRIB_MARKUP_BEGIN
		BO_ATTRIB_MARKUP_ATTRIB(guid)
		BO_ATTRIB_MARKUP_ATTRIB(iTabid)
		BO_ATTRIB_MARKUP_ATTRIB(wScale)
		BO_ATTRIB_MARKUP_ATTRIB(icvHdr)
		BO_ATTRIB_MARKUP_ATTRIB(pnnSel)

		BO_ATTRIB_MARKUP_ATTRIB(fShowBrks)
		BO_ATTRIB_MARKUP_ATTRIB(fDspFmlaSv)
		BO_ATTRIB_MARKUP_ATTRIB(fDspGridSv)
		BO_ATTRIB_MARKUP_ATTRIB(fDspRwColSv)
		BO_ATTRIB_MARKUP_ATTRIB(fDspGutsSv)
		BO_ATTRIB_MARKUP_ATTRIB(fDspZerosSv)
		BO_ATTRIB_MARKUP_ATTRIB(fHorizontal)
		BO_ATTRIB_MARKUP_ATTRIB(fVertical)
		BO_ATTRIB_MARKUP_ATTRIB(fPrintRwCol)
		BO_ATTRIB_MARKUP_ATTRIB(fPrintGrid)
		BO_ATTRIB_MARKUP_ATTRIB(fFitToPage)
		BO_ATTRIB_MARKUP_ATTRIB(fPrintArea)
		BO_ATTRIB_MARKUP_ATTRIB(fOnePrintArea)
		BO_ATTRIB_MARKUP_ATTRIB(fFilterMode)
		BO_ATTRIB_MARKUP_ATTRIB(fEzFilter)
		BO_ATTRIB_MARKUP_ATTRIB(fFrozen)

		BO_ATTRIB_MARKUP_ATTRIB(fFrozenNoSplit)
		BO_ATTRIB_MARKUP_ATTRIB(fSplitV)
		BO_ATTRIB_MARKUP_ATTRIB(fSplitH)
		BO_ATTRIB_MARKUP_ATTRIB(fHiddenRw)
		BO_ATTRIB_MARKUP_ATTRIB(fHiddenCol)
		BO_ATTRIB_MARKUP_ATTRIB(fFilterUnique)
		BO_ATTRIB_MARKUP_ATTRIB(fSheetLayoutView)
		BO_ATTRIB_MARKUP_ATTRIB(fPageLayoutView)
		BO_ATTRIB_MARKUP_ATTRIB(fRuler)

		BO_ATTRIB_MARKUP_ATTRIB(ref8TopLeft)
		BO_ATTRIB_MARKUP_ATTRIB_NAME(operNumX, L"operNumX")
		BO_ATTRIB_MARKUP_ATTRIB_NAME(operNumY, L"operNumY")
		BO_ATTRIB_MARKUP_ATTRIB_NAME(colRPane, L"colRPane")
		BO_ATTRIB_MARKUP_ATTRIB_NAME(rwBPane, L"rwBPane")

		BO_ATTRIB_MARKUP_ATTRIB_NAME(pane_top_left_cell, L"pane_top_left_cell")

	BO_ATTRIB_MARKUP_END

};

} // namespace XLS

