/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "Window1.h"

namespace XLS
{

Window1::Window1()
{
}


Window1::~Window1()
{
}


BaseObjectPtr Window1::clone()
{
	return BaseObjectPtr(new Window1(*this));
}


void Window1::writeFields(CFRecord& record)
{
	record << xWn << yWn << dxWn << dyWn;
	unsigned short flags = 0;
	SETBIT(flags, 0, fHidden);
	SETBIT(flags, 1, fIconic);
	SETBIT(flags, 2, fVeryHidden);
	SETBIT(flags, 3, fDspHScroll);
	SETBIT(flags, 4, fDspVScroll);
	SETBIT(flags, 5, fBotAdornment);
	SETBIT(flags, 6, fNoAFDateGroup);
	record << flags << itabCur << itabFirst << ctabSel << wTabRatio;
}


void Window1::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> xWn >> yWn >> dxWn >> dyWn >> flags >> itabCur >> itabFirst >> ctabSel >> wTabRatio;
	
	fHidden			= GETBIT(flags, 0);
	fIconic			= GETBIT(flags, 1);
	fVeryHidden		= GETBIT(flags, 2);
	fDspHScroll		= GETBIT(flags, 3);
	fDspVScroll		= GETBIT(flags, 4);
	fBotAdornment	= GETBIT(flags, 5);
	fNoAFDateGroup	= GETBIT(flags, 6);
}

int Window1::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"workbookView")
		{
			CP_XML_ATTR(L"activeTab"	, itabCur);
			CP_XML_ATTR(L"firstSheet"	, itabFirst);
			CP_XML_ATTR(L"tabRatio"		, wTabRatio);
			CP_XML_ATTR(L"windowHeight"	, dyWn);
			CP_XML_ATTR(L"windowWidth"	, dxWn);
			CP_XML_ATTR(L"yWindow"		, yWn);
			CP_XML_ATTR(L"xWindow"		, xWn);
			
		}
	}
	return 0;
}


} // namespace XLS

