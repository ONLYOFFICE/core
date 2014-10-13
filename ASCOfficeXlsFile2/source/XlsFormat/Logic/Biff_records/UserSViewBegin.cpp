#include "precompiled_xls.h"
#include "UserSViewBegin.h"
#include <Logic/Biff_structures/CellRangeRef.h>
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

UserSViewBegin::UserSViewBegin()
{
}


UserSViewBegin::~UserSViewBegin()
{
}


BaseObjectPtr UserSViewBegin::clone()
{
	return BaseObjectPtr(new UserSViewBegin(*this));
}


void UserSViewBegin::writeFields(CFRecord& record)
{
	GUID guid_num;
	if(!STR::bstr2guid(guid, guid_num))
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"UserSViewBegin", guid);
	}
	record << guid_num << iTabid;
	record.reserveNunBytes(2); // reserved
	record << wScale << icvHdr;
	record.reserveNunBytes(2); // reserved
	record << pnnSel;
	record.reserveNunBytes(3); // reserved

	unsigned __int16 flags = 0;
	SETBIT(flags, 0, fShowBrks);
	SETBIT(flags, 1, fDspFmlaSv);
	SETBIT(flags, 2, fDspGridSv);
	SETBIT(flags, 3, fDspRwColSv);
	SETBIT(flags, 4, fDspGutsSv);
	SETBIT(flags, 5, fDspZerosSv);
	SETBIT(flags, 6, fHorizontal);
	SETBIT(flags, 7, fVertical);
	SETBIT(flags, 8, fPrintRwCol);
	SETBIT(flags, 9, fPrintGrid);
	SETBIT(flags, 10, fFitToPage);
	SETBIT(flags, 11, fPrintArea);
	SETBIT(flags, 12, fOnePrintArea);
	SETBIT(flags, 13, fFilterMode);
	SETBIT(flags, 14, fEzFilter);
	SETBIT(flags, 15, fFrozen);
	record << flags;

	flags = 0;
	SETBIT(flags, 0, fFrozenNoSplit);
	SETBIT(flags, 1, fSplitV);
	SETBIT(flags, 2, fSplitH);
	SETBIT(flags, 3, fHiddenRw);
	SETBIT(flags, 5, fHiddenCol);
	SETBIT(flags, 9, fFilterUnique);
	SETBIT(flags, 10, fSheetLayoutView);
	SETBIT(flags, 11, fPageLayoutView);
	SETBIT(flags, 13, fRuler);
	record << flags;

	Ref8U top_left(ref8TopLeft); // Will be treated as single cell
	record << top_left;

	record << operNumX << operNumY;
	
	CellRef ref_tmp(pane_top_left_cell);
	if(fSplitV)
	{
		colRPane = 65535;
	}
	else
	{
		colRPane = static_cast<unsigned __int16>(ref_tmp.getColumn());
	}
	if(fSplitH)
	{
		rwBPane = 65535;
	}
	else
	{
		rwBPane = static_cast<unsigned __int16>(ref_tmp.getRow());
	}

	record << colRPane << rwBPane;
}


void UserSViewBegin::readFields(CFRecord& record)
{
	GUID guid_num;
	record >> guid_num >> iTabid;
	guid = STR::guid2bstr(guid_num);
	record.skipNunBytes(2); // reserved
	record >> wScale >> icvHdr;
	record.skipNunBytes(2); // reserved
	record >> pnnSel;
	record.skipNunBytes(3); // reserved
	
	unsigned __int16 flags;
	record >> flags;

	fShowBrks = GETBIT(flags, 0);
	fDspFmlaSv = GETBIT(flags, 1);
	fDspGridSv = GETBIT(flags, 2);
	fDspRwColSv = GETBIT(flags, 3);
	fDspGutsSv = GETBIT(flags, 4);
	fDspZerosSv = GETBIT(flags, 5);
	fHorizontal = GETBIT(flags, 6);
	fVertical = GETBIT(flags, 7);
	fPrintRwCol = GETBIT(flags, 8);
	fPrintGrid = GETBIT(flags, 9);
	fFitToPage = GETBIT(flags, 10);
	fPrintArea = GETBIT(flags, 11);
	fOnePrintArea = GETBIT(flags, 12);
	fFilterMode = GETBIT(flags, 13);
	fEzFilter = GETBIT(flags, 14);
	fFrozen = GETBIT(flags, 15);

	record >> flags;

	Ref8U top_left;
	record >> top_left;
	ref8TopLeft = static_cast<std::wstring >(top_left.getTopLeftCell());

	fFrozenNoSplit = GETBIT(flags, 0);
	fSplitV = GETBIT(flags, 1);
	fSplitH = GETBIT(flags, 2);
	fHiddenRw = GETBIT(flags, 3);
	fHiddenCol = GETBIT(flags, 5);
	fFilterUnique = GETBIT(flags, 9);
	fSheetLayoutView = GETBIT(flags, 10);
	fPageLayoutView = GETBIT(flags, 11);
	fRuler = GETBIT(flags, 13);

	record >> operNumX >> operNumY >> colRPane >> rwBPane;

	pane_top_left_cell = std::wstring (CellRef(fSplitH ? rwBPane : 0 , fSplitV ? colRPane : 0, true, true).toString().c_str());
}

} // namespace XLS

