#include "stdafx.h"
#include "UserBView.h"
#include <Exception/AttributeDataWrong.h>

namespace XLS
{;

UserBView::UserBView()
{
}


UserBView::~UserBView()
{
}


BaseObjectPtr UserBView::clone()
{
	return BaseObjectPtr(new UserBView(*this));
}


void UserBView::writeFields(CFRecord& record)
{
	record.reserveNBytes(4); // unused1
	record << tabId;
	record.reserveNBytes(2); // reserved1
	GUID guid_num;
	if(!STR::bstr2guid(guid, guid_num))
	{
		throw EXCEPT::LE::AttributeDataWrong(L"guid", L"UserBView", guid);
	}
	record << guid_num << x << y << dx << dy << wTabRatio;

	WORD flags1 = 0;
	SETBIT(flags1, 0, fDspFmlaBar);
	SETBIT(flags1, 1, fDspStatus);
	BYTE mdNoteDisp_num;
	if(_bstr_t(L"commNone") == mdNoteDisp)
	{
		mdNoteDisp_num = 0;
	}
	else if(_bstr_t(L"commIndicator") == mdNoteDisp)
	{
		mdNoteDisp_num = 1;
	}
	else if(_bstr_t(L"commIndAndComment") == mdNoteDisp)
	{
		mdNoteDisp_num = 2;
	}
	else
	{
		throw EXCEPT::RT::WrongBiffRecord("Unsupported value of mdNoteDisp.", record.getTypeString());
	}
	SETBITS(flags1, 2, 3, mdNoteDisp_num);
	SETBIT(flags1, 4, fDspHScroll);
	SETBIT(flags1, 5, fDspVScroll);
	SETBIT(flags1, 6, fBotAdornment);
	SETBIT(flags1, 7, fZoom);
	BYTE fHideObj_num;
	if(_bstr_t(L"all") == fHideObj)
	{
		fHideObj_num = 0;
	}
	else if(_bstr_t(L"placeholders") == fHideObj)
	{
		fHideObj_num = 1;
	}
	else if(_bstr_t(L"none") == fHideObj)
	{
		fHideObj_num = 2;
	}
	else
	{
		throw EXCEPT::RT::WrongBiffRecord("Unsupported value of fHideObj.", record.getTypeString());
	}
	SETBITS(flags1, 8, 9, fHideObj_num);
	SETBIT(flags1, 10, fPrintIncl);
	SETBIT(flags1, 11, fRowColIncl);
	SETBIT(flags1, 12, fInvalidTabId);
	SETBIT(flags1, 13, fTimedUpdate);
	SETBIT(flags1, 14, fAllMemChanges);
	SETBIT(flags1, 15, fOnlySync);
	record << flags1;

	record.reserveNBytes(2); // unused2
	WORD flags2 = 0;
	SETBIT(flags2, 0, fPersonalView);
	SETBIT(flags2, 1, fIconic);
	record << flags2;

	if(fPersonalView && !fTimedUpdate)
	{
		wMergeInterval = 0; // Undefined and MUST be ignored.
	}
	if(!fPersonalView)
	{
		wMergeInterval = 65535;
	}

	record << wMergeInterval << st;
}


void UserBView::readFields(CFRecord& record)
{
	record.skipNBytes(4); // unused1
	record >> tabId;
	record.skipNBytes(2); // reserved1
	GUID guid_num;
	record >> guid_num >> x >> y >> dx >> dy >> wTabRatio;
	guid = STR::guid2bstr(guid_num);

	WORD flags1;
	record >> flags1;
	fDspFmlaBar = GETBIT(flags1, 0);
	fDspStatus = GETBIT(flags1, 1);
	BYTE mdNoteDisp_num = GETBITS(flags1, 2, 3);
	switch(mdNoteDisp_num)
	{
		case 0x0:
			mdNoteDisp = _bstr_t(L"commNone");
			break;
		case 0x1:
			mdNoteDisp = _bstr_t(L"commIndicator");
			break;
		case 0x2:
			mdNoteDisp = _bstr_t(L"commIndAndComment");
			break;
		default:
			throw EXCEPT::RT::WrongBiffRecord("Unsupported value of mdNoteDisp.", record.getTypeString());
	}
	fDspHScroll = GETBIT(flags1, 4);
	fDspVScroll = GETBIT(flags1, 5);
	fBotAdornment = GETBIT(flags1, 6);
	fZoom = GETBIT(flags1, 7);
	BYTE fHideObj_num = GETBITS(flags1, 8, 9);
	switch(fHideObj_num)
	{
	case 0x0:
		fHideObj = _bstr_t(L"all");
		break;
	case 0x1:
		fHideObj = _bstr_t(L"placeholders");
		break;
	case 0x2:
		fHideObj = _bstr_t(L"none");
		break;
	default:
		throw EXCEPT::RT::WrongBiffRecord("Unsupported value of fHideObj.", record.getTypeString());
	}
	fPrintIncl = GETBIT(flags1, 10);
	fRowColIncl = GETBIT(flags1, 11);
	fInvalidTabId = GETBIT(flags1, 12);
	fTimedUpdate = GETBIT(flags1, 13);
	fAllMemChanges = GETBIT(flags1, 14);
	fOnlySync = GETBIT(flags1, 15);

	record.skipNBytes(2); // unused2
	WORD flags2;
	record >> flags2;
	fPersonalView = GETBIT(flags2, 0);
	fIconic = GETBIT(flags2, 1);

	record >> wMergeInterval >> st;
}

} // namespace XLS

