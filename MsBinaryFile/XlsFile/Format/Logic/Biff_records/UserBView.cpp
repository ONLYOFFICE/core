/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "UserBView.h"

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

void UserBView::readFields(CFRecord& record)
{
    if(record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record.skipNunBytes(4); // unused1
        _UINT16			tabId_2b;
        record >> tabId_2b;
        tabId = tabId_2b;
        record.skipNunBytes(2); // reserved1

        _GUID_ guid_num;
        _UINT16	wTabRatio_2b;
        record >> guid_num >> x >> y >> dx >> dy >> wTabRatio_2b;
        wTabRatio = wTabRatio_2b;

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
    else
    {
        _GUID_ guid_num;
        record >> x >> y >> dx >> dy >> tabId >> wTabRatio >> guid_num;

        guid = STR::guid2bstr(guid_num);

        record >> wMergeInterval;

        unsigned int flags;
        record >> flags;
        fIconic         = GETBIT(flags, 0);
        fDspHScroll     = GETBIT(flags, 1);
        fDspVScroll     = GETBIT(flags, 2);
        fBotAdornment   = GETBIT(flags, 3);
        fZoom           = GETBIT(flags, 4);
        fDspFmlaBar     = GETBIT(flags, 5);
        fDspStatus      = GETBIT(flags, 6);

        unsigned char mdNoteDisp_num = GETBITS(flags, 7, 8);
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

        unsigned char fHideObj_num = GETBITS(flags, 9, 10);
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
        fPrintIncl		= GETBIT(flags, 11);
        fRowColIncl		= GETBIT(flags, 12);
        fTimedUpdate	= GETBIT(flags, 13);
        fAllMemChanges	= GETBIT(flags, 14);
        fOnlySync		= GETBIT(flags, 15);
        fPersonalView	= GETBIT(flags, 16);
        record >> stName;
    }
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
			CP_XML_ATTR(L"guid",			guid);
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

