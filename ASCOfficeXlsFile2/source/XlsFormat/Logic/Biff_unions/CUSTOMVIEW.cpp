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

#include "CUSTOMVIEW.h"
#include <Logic/Biff_records/UserSViewBegin.h>
#include <Logic/Biff_records/Selection.h>
#include <Logic/Biff_records/HorizontalPageBreaks.h>
#include <Logic/Biff_records/VerticalPageBreaks.h>
#include <Logic/Biff_records/Header.h>
#include <Logic/Biff_records/Footer.h>
#include <Logic/Biff_records/HCenter.h>
#include <Logic/Biff_records/VCenter.h>
#include <Logic/Biff_records/LeftMargin.h>
#include <Logic/Biff_records/RightMargin.h>
#include <Logic/Biff_records/TopMargin.h>
#include <Logic/Biff_records/BottomMargin.h>
#include <Logic/Biff_records/Pls.h>
#include <Logic/Biff_records/Setup.h>
#include <Logic/Biff_records/PrintSize.h>
#include <Logic/Biff_records/HeaderFooter.h>
#include <Logic/Biff_records/UserSViewEnd.h>
#include <Logic/Biff_unions/AUTOFILTER.h>

namespace XLS
{


CUSTOMVIEW::CUSTOMVIEW()
{
}


CUSTOMVIEW::~CUSTOMVIEW()
{
}


BaseObjectPtr CUSTOMVIEW::clone()
{
	return BaseObjectPtr(new CUSTOMVIEW(*this));
}


/*
CUSTOMVIEW = UserSViewBegin *Selection [HorizontalPageBreaks] [VerticalPageBreaks] [Header] 
			[Footer] [HCenter] [VCenter] [LeftMargin] [RightMargin] [TopMargin] [BottomMargin] [Pls] [Setup] 
			[PrintSize] [HeaderFooter] [AUTOFILTER] UserSViewEnd
*/
const bool CUSTOMVIEW::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<UserSViewBegin>())
	{
		return false;
	}
	m_UserSView = elements_.back();
	elements_.pop_back();
	
	int count = proc.repeated<Selection>(0, 0);
	while(count > 0)
	{
		m_arSelection.insert(m_arSelection.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	proc.optional<HorizontalPageBreaks>();
	proc.optional<VerticalPageBreaks>();

	proc.optional<Header>();
	proc.optional<Footer>();
	proc.optional<HCenter>();
	proc.optional<VCenter>();

	proc.optional<LeftMargin>();
	proc.optional<RightMargin>();
	proc.optional<TopMargin>();
	proc.optional<BottomMargin>();

    if (proc.optional<Pls>())
	{
		m_pls = elements_.back();
		elements_.pop_back();
	}

	proc.optional<Setup>();
	proc.optional<PrintSize>();
	proc.optional<HeaderFooter>();

	if (proc.optional<AUTOFILTER>())
	{
		m_AUTOFILTER = elements_.back();
		elements_.pop_back();
	}
	proc.mandatory<UserSViewEnd>();

	return true;
}

int CUSTOMVIEW::serialize(std::wostream & stream)
{
	if (m_UserSView == NULL) return 0;

	UserSViewBegin* userSView = dynamic_cast<UserSViewBegin*>(m_UserSView.get());

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"customSheetView")
		{	
			CP_XML_ATTR(L"guid",			userSView->guid);
			CP_XML_ATTR(L"showGridLines",	userSView->fDspGridSv);

			if (userSView->wScale != 100)	CP_XML_ATTR(L"scale", userSView->wScale);
			if (userSView->icvHdr != 64)	CP_XML_ATTR(L"colorId", userSView->icvHdr);

			if (userSView->fShowBrks)		CP_XML_ATTR(L"showPageBreaks",	true);
			if (userSView->fDspFmlaSv)		CP_XML_ATTR(L"showFormulas",	true);
			if (userSView->fFitToPage)		CP_XML_ATTR(L"fitToPage",		true);
			if (!userSView->fDspRwColSv)	CP_XML_ATTR(L"showRowCol",		false);
			if (!userSView->fDspGutsSv)		CP_XML_ATTR(L"outlineSymbols",	false);
			if (!userSView->fDspZerosSv)	CP_XML_ATTR(L"zeroValues",		false);
			if (!userSView->fPrintArea)		CP_XML_ATTR(L"printArea",		false);
			if (!userSView->fFilterMode)	CP_XML_ATTR(L"filter",			false);
			if (userSView->fEzFilter)		CP_XML_ATTR(L"showAutoFilter",	true);
			if (userSView->fHiddenRw)		CP_XML_ATTR(L"hiddenRows",		true);
			if (userSView->fFilterUnique)	CP_XML_ATTR(L"filterUnique",	true);
 			if (!userSView->fRuler)			CP_XML_ATTR(L"showRuler",	false);
			
			if (userSView->fSheetLayoutView)	CP_XML_ATTR(L"view",	L"pageBreakPreview");
			else if (userSView->fPageLayoutView)CP_XML_ATTR(L"view",	L"pageLayout");
			else								CP_XML_ATTR(L"view",	L"normal");

			if (userSView->ref8TopLeft != L"A1")CP_XML_ATTR(L"topLeftCell",	userSView->ref8TopLeft);

			CP_XML_NODE(L"pane")
			{
  				if (userSView->operNumX != 0)	CP_XML_ATTR(L"xSplit", userSView->operNumX);
  				if (userSView->operNumY != 0)	CP_XML_ATTR(L"ySplit", userSView->operNumY);

				if (userSView->pane_top_left_cell != L"A1")CP_XML_ATTR(L"topLeftCell",	userSView->pane_top_left_cell);

				switch(userSView->pnnSel)
				{
					case PaneType::REVTPNNBOTRIGHT:	CP_XML_ATTR(L"activePane", L"bottomRight");	break;
					case PaneType::REVTPNNTOPRIGHT:	CP_XML_ATTR(L"activePane", L"topRight");	break;
					case PaneType::REVTPNNBOTLEFT:	CP_XML_ATTR(L"activePane", L"bottomLeft");	break;
					//default:						CP_XML_ATTR(L"activePane", L"topLeft"); 					
				}
				if (userSView->fFrozen && userSView->fFrozenNoSplit) 
					CP_XML_ATTR(L"state", L"frozen");
				else if (userSView->fFrozen && !userSView->fFrozenNoSplit) 
					CP_XML_ATTR(L"state", L"frozenSplit");
				else 
					CP_XML_ATTR(L"state", L"split");
			}
			for (int i = 0; i < m_arSelection.size(); i++)
			{
				if (m_arSelection[i] == NULL) continue;
				m_arSelection[i]->serialize(CP_XML_STREAM());
			}
			//todooo пока не понятно как там определяется ref
			//if (m_AUTOFILTER)
			//	m_AUTOFILTER->serialize(CP_XML_STREAM());
		}
	}
	return 0;
}

} // namespace XLS

