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

#include "WorkSheetStream.h"

#include "Biff12_records/CommonRecords.h"
#include "Biff12_records/BeginSheet.h""
#include "Biff12_unions/COLINFOS.h"
#include "Biff12_records/WsDim.h"
#include "Biff12_records/Drawing.h""
#include "Biff12_records/LegacyDrawing.h"
#include "Biff12_records/LegacyDrawingHF.h"
#include "Biff12_unions/HLINKS.h"
#include "Biff12_unions/MERGECELLS.h"
#include "Biff12_unions/CELLTABLE.h"
#include "Biff12_records/WsFmtInfo.h"
#include "Biff12_unions/WSVIEWS2.h"
#include "Biff12_records/Margins.h"
#include "Biff12_records/PrintOptions.h"
#include "Biff12_unions/HEADERFOOTER.h"
#include "Biff12_records/SheetProtectionIso.h"
#include "Biff12_records/SheetProtection.h"
#include "Biff12_unions/LISTPARTS.h"
#include "Biff12_unions/AUTOFILTER.h"
#include "Biff12_unions/SORTSTATE.h"
#include "Biff12_unions/CONDITIONALFORMATTING.h"
#include "Biff12_unions/DVALS.h"
#include "Biff12_unions/OLEOBJECTS.h"
#include "Biff12_unions/ACTIVEXCONTROLS.h"
#include "Biff12_records/WsProp.h"
#include "Biff12_records/BkHim.h"
#include "Biff12_unions/RWBRK.h"
#include "Biff12_unions/COLBRK.h"
#include "Biff12_records/RangeProtectionIso.h"
#include "Biff12_records/RangeProtection.h"
#include "Biff12_unions/DCON.h"
#include "Biff12_unions/FRTWORKSHEET.h"
#include "Biff12_records/EndSheet.h"

using namespace XLS;

namespace XLSB
{;

WorkSheetStream::WorkSheetStream()
{
}

WorkSheetStream::~WorkSheetStream()
{
}

BaseObjectPtr WorkSheetStream::clone()
{
        return BaseObjectPtr(new WorkSheetStream(*this));
}

const bool WorkSheetStream::loadContent(BinProcessor& proc)
{
	int	count = 0;
    std::vector<CellRangeRef>	shared_formulas_locations;
	while (true)
	{
		CFRecordType::TypeId type = proc.getNextRecordType();
		
		if (type == rt_NONE) break;

		switch(type)
		{
            case rt_BeginSheet:
            {
                if (proc.optional<BeginSheet>())
                {
                    m_BrtBeginSheet = elements_.back();
                    elements_.pop_back();
                }
            }break;           

            case rt_BeginColInfos:
            {
                count = proc.repeated<COLINFOS>(0, 0);
                while(count > 0)
                {
                    m_arCOLINFOS.insert(m_arCOLINFOS.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_WsDim:
            {
                if (proc.optional<WsDim>())
                {
                    m_BrtWsDim = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_Drawing:
            {
                if (proc.optional<Drawing>())
                {
                    m_BrtDrawing = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_LegacyDrawing:
            {
                if (proc.optional<LegacyDrawing>())
                {
                    m_BrtLegacyDrawing = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_LegacyDrawingHF:
            {
                if (proc.optional<LegacyDrawingHF>())
                {
                    m_BrtLegacyDrawingHF = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_HLink:
            {
                if (proc.optional<HLINKS>())
                {
                    m_HLINKS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginMergeCells:
            {
                if (proc.optional<MERGECELLS>())
                {
                    m_MERGECELLS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginSheetData:
            {
                /*CELLTABLE cell_table_temlate(shared_formulas_locations);
                if (proc.optional(cell_table_temlate))
                {
                    m_CELLTABLE = elements_.back();
                    elements_.pop_back();
                }*/
                m_SheetaDataPosition = proc.GetRecordPosition();
                while(proc.getNextRecordType() != rt_EndSheetData)
                    proc.SkipRecord(false);
            }break;

            case rt_BeginUserShViews:
            {
                while(proc.getNextRecordType() != rt_EndUserShViews)
                    proc.SkipRecord();
            }break;

            case rt_WsFmtInfo:
            {
                if (proc.optional<WsFmtInfo>())
                {
                    m_BrtWsFmtInfo = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginWsViews:
            {
                if (proc.optional<WSVIEWS2>())
                {
                    m_WSVIEWS2 = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_Margins:
            {
                if (proc.optional<Margins>())
                {
                    m_BrtMargins = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_PageSetup:
            {
                if (proc.optional<PageSetup>())
                {
                    m_BrtPageSetup = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_PrintOptions:
            {
                if (proc.optional<PrintOptions>())
                {
                    m_BrtPrintOptions = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginHeaderFooter:
            {
                if (proc.optional<HEADERFOOTER>())
                {
                    m_HEADERFOOTER = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_SheetProtectionIso:
            {
                if (proc.optional<SheetProtectionIso>())
                {
                    m_BrtSheetProtectionIso = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_SheetProtection:
            {
                if (proc.optional<SheetProtection>())
                {
                    m_BrtSheetProtection = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginListParts:
            {
                if (proc.optional<LISTPARTS>())
                {
                    m_LISTPARTS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginAFilter:
            case rt_ACBegin:
            {
                if (proc.optional<AUTOFILTER>())
                {
                    m_AUTOFILTER = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginSortState:
            {
                if (proc.optional<SORTSTATE>())
                {
                    m_SORTSTATE = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginConditionalFormatting:
            {
                count = proc.repeated<CONDITIONALFORMATTING>(0, 0);
                while(count > 0)
                {
                    m_arCONDITIONALFORMATTING.insert(m_arCONDITIONALFORMATTING.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_BeginDVals:
            {
                if (proc.optional<DVALS>())
                {
                    m_DVALS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginOleObjects:
            {
                if (proc.optional<OLEOBJECTS>())
                {
                    m_OLEOBJECTS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginActiveXControls:
            {
                if (proc.optional<ACTIVEXCONTROLS>())
                {
                    m_ACTIVEXCONTROLS = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_WsProp:
            {
                if (proc.optional<WsProp>())
                {
                    m_BrtWsProp = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BkHim:
            {
                if (proc.optional<BkHim>())
                {
                    m_BrtBkHim = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginRwBrk:
            {
                if (proc.optional<RWBRK>())
                {
                    m_RWBRK = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_BeginColBrk:
            {
                if (proc.optional<COLBRK>())
                {
                    m_COLBRK = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_RangeProtectionIso:
            {
                count = proc.repeated<RangeProtectionIso>(0, 0);
                while(count > 0)
                {
                    m_arBrtRangeProtectionIso.insert(m_arBrtRangeProtectionIso.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_RangeProtection:
            {
                count = proc.repeated<RangeProtection>(0, 0);
                while(count > 0)
                {
                    m_arBrtRangeProtection.insert(m_arBrtRangeProtection.begin(), elements_.back());
                    elements_.pop_back();
                    count--;
                }
            }break;

            case rt_BeginDCon:
            {
                if (proc.optional<DCON>())
                {
                    m_DCON = elements_.back();
                    elements_.pop_back();
                }
            }break;

            case rt_FRTBegin:
            {
                FRTWORKSHEET oFRTWORKSHEET;
                if (proc.optional(oFRTWORKSHEET))
                {
                    if(m_FRTWORKSHEET != nullptr)
                    {
                        if(oFRTWORKSHEET.m_CONDITIONALFORMATTINGS != nullptr)
                            boost::dynamic_pointer_cast<FRTWORKSHEET>(m_FRTWORKSHEET)->m_CONDITIONALFORMATTINGS = oFRTWORKSHEET.m_CONDITIONALFORMATTINGS;

                        if(oFRTWORKSHEET.m_DVALS14 != nullptr)
                            boost::dynamic_pointer_cast<FRTWORKSHEET>(m_FRTWORKSHEET)->m_DVALS14 = oFRTWORKSHEET.m_DVALS14;

                        if(oFRTWORKSHEET.m_SPARKLINEGROUPS != nullptr)
                            boost::dynamic_pointer_cast<FRTWORKSHEET>(m_FRTWORKSHEET)->m_SPARKLINEGROUPS = oFRTWORKSHEET.m_SPARKLINEGROUPS;

                        if(oFRTWORKSHEET.m_SLICERSEX != nullptr)
                            boost::dynamic_pointer_cast<FRTWORKSHEET>(m_FRTWORKSHEET)->m_SLICERSEX = oFRTWORKSHEET.m_SLICERSEX;

                        if(oFRTWORKSHEET.m_TABLESLICERSEX != nullptr)
                            boost::dynamic_pointer_cast<FRTWORKSHEET>(m_FRTWORKSHEET)->m_TABLESLICERSEX = oFRTWORKSHEET.m_TABLESLICERSEX;

                        elements_.pop_back();
                    }
                    else
                    {
                        m_FRTWORKSHEET = elements_.back();
                        elements_.pop_back();
                    }
                }
            }break;

            case rt_EndSheet:
            {
                if (proc.optional<EndSheet>())
                {
                    m_BrtEndSheet = elements_.back();
                    elements_.pop_back();
                }
            }break;

			default://skip					
			{
				proc.SkipRecord();	
			}break;
		}
	}

	return true;
}

} // namespace XLSB
