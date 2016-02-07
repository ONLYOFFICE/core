#pragma once

#include "BiffRecord.h"
#include <Logic/Biff_structures/PaneType.h>

namespace XLS
{


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
	BIFF_BSTR	guid;
	TabId		iTabid;
	BIFF_DWORD	wScale;
	Icv			icvHdr;
	PaneType	pnnSel;

	bool fShowBrks;
	bool fDspFmlaSv;
	bool fDspGridSv;
	bool fDspRwColSv;
	bool fDspGutsSv;
	bool fDspZerosSv;
	bool fHorizontal;
	bool fVertical;
	bool fPrintRwCol;
	bool fPrintGrid;
	bool fFitToPage;
	bool fPrintArea;
	bool fOnePrintArea;
	bool fFilterMode;
	bool fEzFilter;
	bool fFrozen;

	bool fFrozenNoSplit;
	bool fSplitV;
	bool fSplitH;
	bool fHiddenRw;
	bool fHiddenCol;
	bool fFilterUnique;
	bool fSheetLayoutView;
	bool fPageLayoutView;
	bool fRuler;

	std::wstring	ref8TopLeft;
	BIFF_DOUBLE		operNumX;
	BIFF_DOUBLE		operNumY;
	
	ForwardOnlyParam<unsigned short> colRPane;
	ForwardOnlyParam<unsigned short> rwBPane;

	std::wstring	pane_top_left_cell;

};

} // namespace XLS

