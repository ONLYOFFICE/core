
#include "UserBView.h"
//#include <Exception/AttributeDataWrong.h>

namespace XLS
{

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
	record.reserveNunBytes(4); // unused1
	record << tabId;
	record.reserveNunBytes(2); // reserved1
	_GUID_ guid_num;
	if(!STR::bstr2guid(guid, guid_num))
	{
		throw;// EXCEPT::LE::AttributeDataWrong(L"guid", L"UserBView", guid);
	}
	record << guid_num << x << y << dx << dy << wTabRatio;

	unsigned short flags1 = 0;
	SETBIT(flags1, 0, fDspFmlaBar);
	SETBIT(flags1, 1, fDspStatus);
	unsigned char mdNoteDisp_num;
	if(mdNoteDisp == std::wstring (L"commNone"))
	{
		mdNoteDisp_num = 0;
	}
	else if(mdNoteDisp == std::wstring (L"commIndicator"))
	{
		mdNoteDisp_num = 1;
	}
	else if(mdNoteDisp == std::wstring (L"commIndAndComment"))
	{
		mdNoteDisp_num = 2;
	}
	else
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported value of mdNoteDisp.", record.getTypeString());
	}
	SETBITS(flags1, 2, 3, mdNoteDisp_num);
	SETBIT(flags1, 4, fDspHScroll);
	SETBIT(flags1, 5, fDspVScroll);
	SETBIT(flags1, 6, fBotAdornment);
	SETBIT(flags1, 7, fZoom);
	
	unsigned char fHideObj_num;
	
	if(fHideObj == std::wstring (L"all"))
	{
		fHideObj_num = 0;
	}
	else if(fHideObj == std::wstring (L"placeholders"))
	{
		fHideObj_num = 1;
	}
	else if(fHideObj == std::wstring (L"none"))
	{
		fHideObj_num = 2;
	}
	else
	{
		throw;// EXCEPT::RT::WrongBiffRecord("Unsupported value of fHideObj.", record.getTypeString());
	}
	SETBITS(flags1, 8, 9, fHideObj_num);
	SETBIT(flags1, 10, fPrintIncl);
	SETBIT(flags1, 11, fRowColIncl);
	SETBIT(flags1, 12, fInvalidTabId);
	SETBIT(flags1, 13, fTimedUpdate);
	SETBIT(flags1, 14, fAllMemChanges);
	SETBIT(flags1, 15, fOnlySync);
	record << flags1;

	record.reserveNunBytes(2); // unused2
	unsigned short flags2 = 0;
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
	record.skipNunBytes(4); // unused1
	record >> tabId;
	record.skipNunBytes(2); // reserved1
	
	_GUID_ guid_num;
	record >> guid_num >> x >> y >> dx >> dy >> wTabRatio;
	guid = STR::guid2bstr(guid_num);

	unsigned short flags1;
	record >> flags1;
	fDspFmlaBar	= GETBIT(flags1, 0);
	fDspStatus	= GETBIT(flags1, 1);
	
	unsigned char mdNoteDisp_num = GETBITS(flags1, 2, 3);
	switch(mdNoteDisp_num)
	{
		case 0x0:
			mdNoteDisp = std::wstring (L"commNone");
			break;
		case 0x2:
			mdNoteDisp = std::wstring (L"commIndAndComment");
			break;
		case 0x1:
		default:
			mdNoteDisp = std::wstring (L"commIndicator");
			break;
	}
	fDspHScroll		= GETBIT(flags1, 4);
	fDspVScroll		= GETBIT(flags1, 5);
	fBotAdornment	= GETBIT(flags1, 6);
	fZoom			= GETBIT(flags1, 7);
	
	unsigned char fHideObj_num = GETBITS(flags1, 8, 9);
	switch(fHideObj_num)
	{
	case 0x1:
		fHideObj = std::wstring (L"placeholders");
		break;
	case 0x2:
		fHideObj = std::wstring (L"none");
		break;
	case 0x0:
	default:
		fHideObj = std::wstring (L"all");
		break;
	}
	fPrintIncl		= GETBIT(flags1, 10);
	fRowColIncl		= GETBIT(flags1, 11);
	fInvalidTabId	= GETBIT(flags1, 12);
	fTimedUpdate	= GETBIT(flags1, 13);
	fAllMemChanges	= GETBIT(flags1, 14);
	fOnlySync		= GETBIT(flags1, 15);

	record.skipNunBytes(2); // unused2
	unsigned short flags2;
	record >> flags2;
	
	fPersonalView	= GETBIT(flags2, 0);
	fIconic			= GETBIT(flags2, 1);

	record >> wMergeInterval >> st;
}

int UserBView::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"customWorkbookView")
		{	
			if (!st.value().empty())
			{
				CP_XML_ATTR(L"name", st.value());
			}
			CP_XML_ATTR(L"windowWidth",		dx);
			CP_XML_ATTR(L"windowHeight",	dy);
			CP_XML_ATTR(L"activeSheetId",	tabId);
			CP_XML_ATTR(L"guid",			*guid.value());
			CP_XML_ATTR(L"mergeInterval",	wMergeInterval);

			if (fTimedUpdate)	CP_XML_ATTR(L"autoUpdate"		,	true);
			if (fAllMemChanges)	CP_XML_ATTR(L"changesSavedWin"	,	true);
			if (!fRowColIncl)	CP_XML_ATTR(L"includeHiddenRowCol",	false);
			if (!fPrintIncl)	CP_XML_ATTR(L"includePrintSettings",false);
			if (fZoom)			CP_XML_ATTR(L"maximized"		,	true);
			if (fIconic)		CP_XML_ATTR(L"minimized"		,	true);
			if (fOnlySync)		CP_XML_ATTR(L"onlySync"			,	true);
			if (fPersonalView)	CP_XML_ATTR(L"personalView"		,	true);
			if (!fDspFmlaBar)	CP_XML_ATTR(L"showFormulaBar"	,	false);
			if (!fDspHScroll)	CP_XML_ATTR(L"showHorizontalScroll",false);
			if (!fDspVScroll)	CP_XML_ATTR(L"showVerticalScroll",	false);
			if (!fBotAdornment)	CP_XML_ATTR(L"showSheetTabs"	,	false);
			if (!fDspStatus)	CP_XML_ATTR(L"showStatusbar"	,	false);

			if (mdNoteDisp != L"commIndicator")	CP_XML_ATTR(L"showComments", mdNoteDisp);
			
			if (fHideObj != L"all")	CP_XML_ATTR(L"showObjects"	,	fHideObj);
			if (wTabRatio != 600)	CP_XML_ATTR(L"tabRatio"		,	wTabRatio);
			if (x != 0)				CP_XML_ATTR(L"xWindow"		,	x);
			if (y != 0)				CP_XML_ATTR(L"yWindow"		,	y);
		}
	}
	return 0;
}

} // namespace XLS

