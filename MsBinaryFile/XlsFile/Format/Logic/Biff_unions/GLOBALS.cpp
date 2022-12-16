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

#include "GLOBALS.h"

#include "../Biff_records/DefColWidth.h"
#include "../Biff_records/DxGCol.h"
#include "../Biff_records/Protect.h"
#include "../Biff_records/CalcMode.h"
#include "../Biff_records/CalcCount.h"
#include "../Biff_records/CalcRefMode.h"
#include "../Biff_records/CalcIter.h"
#include "../Biff_records/CalcDelta.h"
#include "../Biff_records/CalcSaveRecalc.h"
#include "../Biff_records/PrintRowCol.h"
#include "../Biff_records/PrintGrid.h"
#include "../Biff_records/GridSet.h"
#include "../Biff_records/Guts.h"
#include "../Biff_records/DefaultRowHeight.h"
#include "../Biff_records/WsBool.h"
#include "../Biff_records/Sync.h"
#include "../Biff_records/LPr.h"
#include "../Biff_records/HorizontalPageBreaks.h"
#include "../Biff_records/VerticalPageBreaks.h"
#include "../Biff_records/Country.h"

namespace XLS
{
GLOBALS::GLOBALS() : is_dialog(false)
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
			case rt_CalcMode:
			{
				if (proc.optional<CalcMode>())
				{
					m_CalcMode = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_CalcCount:		proc.optional<CalcCount>();			break;
			case rt_CalcRefMode:
			{
				if (proc.optional<CalcRefMode>())
				{
					m_CalcRefMode = elements_.back();
					elements_.pop_back();
				}
			}break;
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
			
			case rt_HorizontalPageBreaks:
			{
				if (proc.optional<HorizontalPageBreaks>())
				{
					m_HorizontalPageBreaks = elements_.back();
					elements_.pop_back();
				}
			}break;
			case rt_VerticalPageBreaks:
			{
				if (proc.optional<VerticalPageBreaks>())
				{
					m_VerticalPageBreaks = elements_.back();
					elements_.pop_back();
				}
			}break;
			default://unknown .... back	upper		
				return true;
		}
	}	

	return true;
}
int GLOBALS::serialize_calcPr(std::wostream & stream)
{
	if (!m_CalcMode) return 0;

	CP_XML_WRITER(stream)
	{
		CP_XML_NODE(L"sheetCalcPr")
		{
			XLS::CalcMode *calcMode = dynamic_cast<XLS::CalcMode *>(m_CalcMode.get());
			CP_XML_ATTR(L"fullCalcOnLoad", calcMode->nAutoRecalc > 0 ? 1 : 0);
		}
	}
	return 0;
}
int GLOBALS::serialize_formatPr(std::wostream & stream)
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
			if (m_DefColWidth)
			{
				DefColWidth* def_col = dynamic_cast<DefColWidth*>(m_DefColWidth.get());
				CP_XML_ATTR(L"baseColWidth", def_col->cchdefColWidth);
			}		
			if (m_DxGCol)
			{
				DxGCol* def_col = dynamic_cast<DxGCol*>(m_DxGCol.get());
				CP_XML_ATTR(L"defaultColWidth", def_col->dxgCol / 256.);
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

