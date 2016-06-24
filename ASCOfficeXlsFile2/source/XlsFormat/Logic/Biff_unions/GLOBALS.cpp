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

#include "GLOBALS.h"
#include <Logic/Biff_records/DefColWidth.h>
#include <Logic/Biff_records/DxGCol.h>

#include <Logic/Biff_records/Protect.h>
#include <Logic/Biff_records/CalcMode.h>
#include <Logic/Biff_records/CalcCount.h>
#include <Logic/Biff_records/CalcRefMode.h>
#include <Logic/Biff_records/CalcIter.h>
#include <Logic/Biff_records/CalcDelta.h>
#include <Logic/Biff_records/CalcSaveRecalc.h>
#include <Logic/Biff_records/PrintRowCol.h>
#include <Logic/Biff_records/PrintGrid.h>
#include <Logic/Biff_records/GridSet.h>
#include <Logic/Biff_records/Guts.h>
#include <Logic/Biff_records/DefaultRowHeight.h>
#include <Logic/Biff_records/WsBool.h>
#include <Logic/Biff_records/Sync.h>
#include <Logic/Biff_records/LPr.h>
#include <Logic/Biff_records/HorizontalPageBreaks.h>
#include <Logic/Biff_records/VerticalPageBreaks.h>
#include <Logic/Biff_records/Country.h>

namespace XLS
{


GLOBALS::GLOBALS(const bool is_dialog_sheet)
:	is_dialog(is_dialog_sheet)
{
}


GLOBALS::~GLOBALS()
{
}


BaseObjectPtr GLOBALS::clone()
{
	return BaseObjectPtr(new GLOBALS(*this));
}

/*
GLOBALS = CalcMode CalcCount CalcRefMode CalcIter CalcDelta CalcSaveRecalc PrintRowCol PrintGrid GridSet 
			Guts DefaultRowHeight WsBool [Sync] [LPr] [HorizontalPageBreaks] [VerticalPageBreaks]
*/	
const bool GLOBALS::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();

	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;

		switch(type)
		{
			case rt_CalcMode:		proc.optional<CalcMode>();			break;
			case rt_CalcCount:		proc.optional<CalcCount>();			break;
			case rt_CalcRefMode:	proc.optional<CalcRefMode>();		break;
			case rt_CalcIter:		proc.optional<CalcIter>();			break;
			case rt_CalcDelta:		proc.optional<CalcDelta>();			break;
			case rt_CalcSaveRecalc:	proc.optional<CalcSaveRecalc>();	break;
			case rt_PrintRowCol:	proc.optional<PrintRowCol>();		break;
			case rt_PrintGrid:		proc.optional<PrintGrid>();			break;
			case rt_GridSet:		proc.optional<GridSet>();			break;
			case rt_Sync:			proc.optional<Sync>();				break;
			case rt_LPr:			proc.optional<LPr>();				break;
			case rt_Guts:
			{
				if (proc.optional<Guts>())
				{
					m_Guts = elements_.back();
					elements_.pop_back();
				}
			}break;			
			case rt_DefaultRowHeight:
			{
				if (proc.optional<DefaultRowHeight>())
				{
					m_DefaultRowHeight = elements_.back();
					elements_.pop_back();
				}
			}break;	
			case rt_WsBool:
			{
				WsBool wsbool(is_dialog);
				if (proc.optional(wsbool)) // The most interesting
				{
					m_WsBool = elements_.back();
					elements_.pop_back();
				}
			}break;				
			
			case rt_HorizontalPageBreaks:	proc.optional<HorizontalPageBreaks>();	break;
			case rt_VerticalPageBreaks:		proc.optional<VerticalPageBreaks>();	break;

			default://unknown .... back	upper		
				return true;
		}
	}	

	return true;
}

int GLOBALS::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"sheetFormatPr")
		{
			if (m_DefaultRowHeight)
			{
				DefaultRowHeight* def_row = dynamic_cast<DefaultRowHeight*>(m_DefaultRowHeight.get());
				if (def_row)
				{
					CP_XML_ATTR(L"defaultRowHeight", def_row->miyRw / 20.);
					if(!def_row->fDyZero)
					{
						CP_XML_ATTR(L"customHeight", true);
					}
					else 
					{
						CP_XML_ATTR(L"zeroHeight", true);
					}
					if (def_row->fExAsc)	CP_XML_ATTR(L"thickTop", true);
					if (def_row->fExDsc)	CP_XML_ATTR(L"thickBottom", true);
				}

			}
			else
			{
				CP_XML_ATTR(L"defaultRowHeight", 14.4);
			}
			if (m_DxGCol)
			{
				DxGCol* def_col = dynamic_cast<DxGCol*>(m_DxGCol.get());
				CP_XML_ATTR(L"defaultColWidth", def_col->dxgCol / 256.);
			}
			else if (m_DefColWidth)
			{
				DefColWidth* def_col = dynamic_cast<DefColWidth*>(m_DefColWidth.get());
				CP_XML_ATTR(L"defaultColWidth", def_col->cchdefColWidth);
			}
			if (m_Guts)
			{
				Guts * guts= dynamic_cast<Guts*>(m_Guts.get());

				if (guts->iLevelRwMac > 0)
					CP_XML_ATTR(L"outlineLevelRow", guts->iLevelRwMac);
				
				if (guts->iLevelColMac > 0)
					CP_XML_ATTR(L"outlineLevelCol", guts->iLevelColMac);
			}
		}
	}
	return 0;
}
} // namespace XLS

