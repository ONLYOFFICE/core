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

	static const ElementType	type = typeUserSViewBegin;

//-----------------------------
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

};

} // namespace XLS

